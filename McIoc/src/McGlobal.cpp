#include "McIoc/McGlobal.h"

//#include <private/qvariant_p.h>
//#include <private/qmetatype_p.h>

#include <QEventLoop>
#include <QTimer>
#include <QElapsedTimer>
#include <QDir>
#include <QCoreApplication>
#include <QUrl>
#include <QMetaObject>
#include <QMetaClassInfo>
#include <QDebug>

#include "McIoc/ApplicationContext/impl/McAnnotationApplicationContext.h"
#include "McIoc/BeanDefinition/IMcBeanDefinition.h"

namespace McPrivate {

using StartUpFuncs = QMap<int, QVector<Mc::StartUpFunction>>;
MC_GLOBAL_STATIC(StartUpFuncs, preRFuncs)
using VFuncs = QMap<int, QVector<Mc::CleanUpFunction>>;
MC_GLOBAL_STATIC(VFuncs, postRFuncs)

int iocStaticInit()
{
    qAddPreRoutine([](){
        StartUpFuncs funcs = *preRFuncs;
        auto keys = funcs.keys();
        for(int i = keys.length() - 1; i >= 0; --i) {
            auto list = funcs.value(keys.value(i));
            for(int j = 0; j < list.length(); ++j) {
                list.at(j)();
            }
        }
#if QT_VERSION < QT_VERSION_CHECK(5, 14, 0)
        Q_QGS_preRFuncs::guard.store(QtGlobalStatic::Initialized);
#else
        Q_QGS_preRFuncs::guard.storeRelaxed(QtGlobalStatic::Initialized);
#endif
    });
    qAddPostRoutine([](){
        VFuncs funcs = *postRFuncs;
        auto keys = funcs.keys();
        for(int i = keys.length() - 1; i >= 0; --i) {
            auto list = funcs.value(keys.value(i));
            for(int j = 0; j < list.length(); ++j) {
                list.at(j)();
            }
        }
#if QT_VERSION < QT_VERSION_CHECK(5, 14, 0)
        Q_QGS_postRFuncs::guard.store(QtGlobalStatic::Initialized);
#else
        Q_QGS_postRFuncs::guard.storeRelaxed(QtGlobalStatic::Initialized);
#endif
    });
    return 1;
}
#ifdef Q_CC_GNU
static void __attribute__((constructor)) __iocStaticInit__(void)
{
    iocStaticInit();
}
#elif Q_CC_MSVC
#include <corecrt_startup.h>
#define SECNAME ".CRT$XCY"
#pragma section(SECNAME, long, read)
int __iocStaticInit__()
{
    iocStaticInit();
    return 0;
}
__declspec(allocate(SECNAME)) _PIFV dummy[] = {__iocStaticInit__};
#else
Q_CONSTRUCTOR_FUNCTION(iocStaticInit)
#endif

static const void *constData(const QVariant::Private &d)
{
    return d.is_shared ? d.data.shared->ptr : reinterpret_cast<const void *>(&d.data.c);
}

static void customConstruct(QVariant::Private *d, const void *copy)
{
    const QMetaType type(d->type);
    const uint size = type.sizeOf();
    if (!size) {
        qWarning("Trying to construct an instance of an invalid type, type id: %i", d->type);
        d->type = QMetaType::UnknownType;
        return;
    }

    // this logic should match with QVariantIntegrator::CanUseInternalSpace
    if (size <= sizeof(QVariant::Private::Data)
            && (type.flags() & (QMetaType::MovableType | QMetaType::IsEnumeration))) {
        type.construct(&d->data.ptr, copy);
        d->is_null = d->data.ptr == nullptr;
        d->is_shared = false;
    } else {
        // Private::Data contains long long, and long double is the biggest standard type.
        const size_t maxAlignment =
            qMax(Q_ALIGNOF(QVariant::Private::Data), Q_ALIGNOF(long double));
        const size_t s = sizeof(QVariant::PrivateShared);
        const size_t offset = s + ((s * maxAlignment - s) % maxAlignment);
        void *data = operator new(offset + size);
        void *ptr = static_cast<char *>(data) + offset;
        type.construct(ptr, copy);
        d->is_null = ptr == nullptr;
        d->is_shared = true;
        d->data.shared = new (data) QVariant::PrivateShared(ptr);
    }
}

static void customClear(QVariant::Private *d)
{
    if (!d->is_shared) {
        QMetaType::destruct(d->type, &d->data.ptr);
    } else {
        QMetaType::destruct(d->type, d->data.shared->ptr);
        d->data.shared->~PrivateShared();
        operator delete(d->data.shared);
    }
}

static bool customIsNull(const QVariant::Private *d)
{
    if (d->is_null)
        return true;
    const char *const typeName = QMetaType::typeName(d->type);
    if (Q_UNLIKELY(!typeName) && Q_LIKELY(!QMetaType::isRegistered(d->type)))
        qFatal("QVariant::isNull: type %d unknown to QVariant.", d->type);
    uint typeNameLen = qstrlen(typeName);
    if (typeNameLen > 0 && typeName[typeNameLen - 1] == '*') {
        const void *d_ptr = d->is_shared ? d->data.shared->ptr : &(d->data.ptr);
        return *static_cast<void *const *>(d_ptr) == nullptr;
    }
    return false;
}

static bool customCompare(const QVariant::Private *a, const QVariant::Private *b)
{
    const char *const typeName = QMetaType::typeName(a->type);
    if (Q_UNLIKELY(!typeName) && Q_LIKELY(!QMetaType::isRegistered(a->type)))
        qFatal("QVariant::compare: type %d unknown to QVariant.", a->type);

    const void *a_ptr = a->is_shared ? a->data.shared->ptr : &(a->data.ptr);
    const void *b_ptr = b->is_shared ? b->data.shared->ptr : &(b->data.ptr);

    uint typeNameLen = qstrlen(typeName);
    if (typeNameLen > 0 && typeName[typeNameLen - 1] == '*')
        return *static_cast<void *const *>(a_ptr) == *static_cast<void *const *>(b_ptr);

    if (a->is_null && b->is_null)
        return true;

    return !memcmp(a_ptr, b_ptr, QMetaType::sizeOf(a->type));
}

//static bool customConvert(const QVariant::Private *d, int t, void *result, bool *ok)
//{
//    if (d->type >= QMetaType::User || t >= QMetaType::User) {
//        if (QMetaType::convert(constData(*d), d->type, result, t)) {
//            if (ok)
//                *ok = true;
//            return true;
//        }
//    }
//    auto kernelHandler = qcoreVariantHandler();
//    return kernelHandler->convert(d, t, result, ok);
//}

#if !defined(QT_NO_DEBUG_STREAM)
static void customStreamDebug(QDebug dbg, const QVariant &variant) {
#ifndef QT_BOOTSTRAPPED
    QMetaType::TypeFlags flags = QMetaType::typeFlags(variant.userType());
    if (flags & QMetaType::PointerToQObject)
        dbg.nospace() << qvariant_cast<QObject*>(variant);
#else
    Q_UNUSED(dbg);
    Q_UNUSED(variant);
#endif
}
#endif

//const QVariant::Handler mc_custom_variant_handler = {
//    customConstruct,
//    customClear,
//    customIsNull,
//#ifndef QT_NO_DATASTREAM
//    nullptr,
//    nullptr,
//#endif
//    customCompare,
//    customConvert,
//    nullptr,
//#if !defined(QT_NO_DEBUG_STREAM)
//    customStreamDebug
//#else
//    nullptr
//#endif
//};

MC_STATIC(Mc::RoutinePriority::Max)
auto pId = qRegisterMetaType<QObject *>();
auto sId = qRegisterMetaType<QObjectPtr>();
McMetaTypeId::addQObjectPointerIds(pId, sId);
McMetaTypeId::addSharedPointerId(sId, pId);
qRegisterMetaType<QObjectPtr>(MC_STRINGIFY(QObjectPtr));
qRegisterMetaType<QObjectPtr>(MC_STRINGIFY(QObjectConstPtrRef));

//auto kernelHandler = qcoreVariantHandler();
//QVariantPrivate::registerHandler(QModulesPrivate::Core, kernelHandler);
//QVariantPrivate::registerHandler(QModulesPrivate::Unknown, &mc_custom_variant_handler);
MC_STATIC_END

QString getBeanName(const QMetaObject *metaObj) noexcept
{
    QString beanName;
    auto beanNameIndex = metaObj->indexOfClassInfo(MC_BEANNAME_TAG);
    if(beanNameIndex == -1) {
        beanName = metaObj->className();
        Q_ASSERT(!beanName.isEmpty());
        auto firstChar = beanName.at(0);
        firstChar = firstChar.toLower();
        beanName.remove(0, 1);
        beanName = firstChar + beanName;
    } else {
        auto beanNameClassInfo = metaObj->classInfo(beanNameIndex);
        beanName = beanNameClassInfo.value();
    }
    return beanName;
}

} // namespace McPrivate

McCustomEvent::~McCustomEvent() noexcept
{
}

namespace Mc {

bool waitForExecFunc(const std::function<bool()> &func, qint64 timeout) noexcept 
{
    QEventLoop loop;
    QTimer timer;
    timer.setInterval(100);
    QElapsedTimer stopWatcher;
    bool ret = true;
    
    QObject::connect(&timer, &QTimer::timeout, [&timer, &stopWatcher, &loop, &ret, &func, &timeout](){
        timer.stop();
        if((ret = func())) {
            loop.quit();
            return;
        }
        if(timeout != -1 && stopWatcher.elapsed() > timeout) {
            loop.quit();
            return;
        }
        timer.start();
    });
    
    stopWatcher.start();
    timer.start();
    loop.exec();
    return ret;
}

QString toAbsolutePath(const QString &path) noexcept
{
    QString dstPath = QDir::toNativeSeparators(path);
    QString sepDot = ".";
    QString sepDotDot = "..";
    sepDot += QDir::separator();
    sepDotDot += QDir::separator();
    if(dstPath.startsWith(sepDot) || dstPath.startsWith(sepDotDot)) {
        dstPath = qApp->applicationDirPath() + QDir::separator() + dstPath;
    } else {
        QUrl url(path);
        if(url.isLocalFile()) {
            dstPath = url.toLocalFile();
            dstPath = QDir::toNativeSeparators(dstPath);
            if(!QDir::isAbsolutePath(dstPath)) {
                dstPath = qApp->applicationDirPath() + QDir::separator() + dstPath;
            }
            url = QUrl::fromLocalFile(dstPath);
        }
        dstPath = url.toString();
    }
    dstPath = QDir::cleanPath(dstPath);
    return dstPath;
}

QList<QString> getAllComponent(IMcApplicationContextConstPtrRef appCtx) noexcept
{
    if (!appCtx) {
        qCritical() << "Please call initContainer to initialize container first";
        return QList<QString>();
    }
    QList<QString> components;
    QHash<QString, IMcBeanDefinitionPtr> beanDefinitions = appCtx->getBeanDefinitions();
    for (auto itr = beanDefinitions.cbegin(); itr != beanDefinitions.cend(); ++itr) {
        auto beanDefinition = itr.value();
        if (!isComponent(beanDefinition->getBeanMetaObject()))
            continue;
        components.append(itr.key());
    }
    return components;
}

QList<QString> getComponents(IMcApplicationContextConstPtrRef appCtx, const QString &componentType) noexcept 
{
    if (!appCtx) {
        qCritical() << "Please call initContainer to initialize container first";
        return QList<QString>();
    }
    QList<QString> components;
    QHash<QString, IMcBeanDefinitionPtr> beanDefinitions = appCtx->getBeanDefinitions();
    for (auto itr = beanDefinitions.cbegin(); itr != beanDefinitions.cend(); ++itr) {
        auto beanDefinition = itr.value();
        if (!isComponentType(beanDefinition->getBeanMetaObject(), componentType))
            continue;
        components.append(itr.key());
    }
    return components;
}

bool isComponent(const QMetaObject *metaObj) noexcept
{
    if(!metaObj) {
        return false;
    }
    int classInfoCount = metaObj->classInfoCount();
    for (int i = 0; i < classInfoCount; ++i) {
        auto classInfo = metaObj->classInfo(i);
        if (qstrcmp(classInfo.name(), MC_COMPONENT_TAG) != 0)
            continue;
        return true;
    }
    return false;
}

bool isComponentType(const QMetaObject *metaObj, const QString &type) noexcept 
{
    if(!metaObj) {
        return false;
    }
    int classInfoCount = metaObj->classInfoCount();
    for (int i = 0; i < classInfoCount; ++i) {
        auto classInfo = metaObj->classInfo(i);
        if (qstrcmp(classInfo.name(), MC_COMPONENT_TAG) != 0)
            continue;
        return classInfo.value() == type;
    }
    return false;
}

bool isContainedTag(const QString &tags, const QString &tag) noexcept
{
#if QT_VERSION < QT_VERSION_CHECK(5, 15, 0)
    auto tagList = tags.split(' ', QString::SkipEmptyParts);
#else
    auto tagList = tags.split(' ', Qt::SkipEmptyParts);
#endif
    for (auto t : tagList) {
        if (t == tag) {
            return true;
        }
    }
    return false;
}

QObject *getObject(IMcApplicationContext *appCtx, const QString &beanName) noexcept
{
    auto bean = appCtx->getBeanPointer(beanName);
    if (bean == nullptr) {
        bean = appCtx->getBean(beanName).data();
    }
    if (bean == nullptr) {
        qWarning() << "not exists bean:" << beanName;
    }
    return bean;
}

void addPreRoutine(int priority, const StartUpFunction &func) noexcept
{
    McPrivate::StartUpFuncs *funcs = McPrivate::preRFuncs;
    if(!funcs) {
        return;
    }
    if (QCoreApplication::instance()) {
        func();
    }
    (*funcs)[priority].prepend(func);
}

void addPostRoutine(int priority, const CleanUpFunction &func) noexcept
{
    McPrivate::VFuncs *funcs = McPrivate::postRFuncs;
    if(!funcs) {
        return;
    }
    (*funcs)[priority].prepend(func);
}

namespace Ioc {

void connect(const QString &beanName,
             const QString &sender,
             const QString &signal,
             const QString &receiver,
             const QString &slot,
             Qt::ConnectionType type) noexcept
{
    McAnnotationApplicationContext::addConnect(beanName, sender, signal, receiver, slot, type);
}

void connect(const QMetaObject *metaObj,
             const QString &sender,
             const QString &signal,
             const QString &receiver,
             const QString &slot,
             Qt::ConnectionType type) noexcept
{
    Q_ASSERT(metaObj != nullptr);
    QString beanName = McPrivate::getBeanName(metaObj);
    connect(beanName, sender, signal, receiver, slot, type);
}

QMetaObject::Connection connect(IMcApplicationContext *appCtx,
                                const QString &sender,
                                const QString &signal,
                                const QString &receiver,
                                const QString &slot,
                                Qt::ConnectionType type) noexcept
{
    Q_ASSERT(appCtx != nullptr);
    auto *receiverObj = appCtx->getBeanPointer<QObject>(receiver);
    if (receiverObj == nullptr) {
        receiverObj = appCtx->getBean<QObject>(receiver).data();
    }
    if (receiverObj == nullptr) {
        qCritical("not exists bean '%s'", qPrintable(receiver));
        return QMetaObject::Connection();
    }
    return connect(appCtx, sender, signal, receiverObj, slot, type);
}

QMetaObject::Connection connect(IMcApplicationContext *appCtx,
                                const QString &sender,
                                const QString &signal,
                                QObject *receiver,
                                const QString &slot,
                                Qt::ConnectionType type) noexcept
{
    Q_ASSERT(appCtx != nullptr);
    Q_ASSERT(receiver != nullptr);
    auto senderObj = appCtx->getBeanPointer<QObject>(sender);
    if (senderObj == nullptr) {
        senderObj = appCtx->getBean<QObject>(sender).data();
    }
    if (senderObj == nullptr) {
        qCritical("not exists bean '%s'", qPrintable(sender));
        return QMetaObject::Connection();
    }
    QString signalStr = signal;
    if (!signalStr.startsWith(QString::number(QSIGNAL_CODE))) {
        signalStr = QString::number(QSIGNAL_CODE) + signalStr;
    }
    auto slotStr = slot;
    if (!slotStr.startsWith(QString::number(QSLOT_CODE))) {
        slotStr = QString::number(QSLOT_CODE) + slotStr;
    }
    return QObject::connect(senderObj,
                            signalStr.toLocal8Bit(),
                            receiver,
                            slotStr.toLocal8Bit(),
                            type);
}

bool disconnect(IMcApplicationContext *appCtx,
                const QString &sender,
                const QString &signal,
                const QString &receiver,
                const QString &slot) noexcept
{
    Q_ASSERT(appCtx != nullptr);
    auto *receiverObj = appCtx->getBeanPointer<QObject>(receiver);
    if (receiverObj == nullptr) {
        receiverObj = appCtx->getBean<QObject>(receiver).data();
    }
    return disconnect(appCtx, sender, signal, receiverObj, slot);
}

bool disconnect(IMcApplicationContext *appCtx,
                const QString &sender,
                const QString &signal,
                QObject *receiver,
                const QString &slot) noexcept
{
    Q_ASSERT(appCtx != nullptr);
    auto senderObj = appCtx->getBeanPointer<QObject>(sender);
    if (senderObj == nullptr) {
        senderObj = appCtx->getBean<QObject>(sender).data();
    }
    if (senderObj == nullptr) {
        qCritical("not exists bean '%s'", qPrintable(sender));
        return false;
    }
    const char *sig = nullptr;
    const char *slt = nullptr;
    QString signalStr = signal;
    if (!signalStr.isEmpty() && !signalStr.startsWith(QString::number(QSIGNAL_CODE))) {
        signalStr = QString::number(QSIGNAL_CODE) + signalStr;
    }
    if (!signalStr.isEmpty()) {
        sig = signalStr.toLocal8Bit();
    }
    auto slotStr = slot;
    if (!slotStr.isEmpty() && !slotStr.startsWith(QString::number(QSLOT_CODE))) {
        slotStr = QString::number(QSLOT_CODE) + slotStr;
    }
    if (!slotStr.isEmpty()) {
        slt = slotStr.toLocal8Bit();
    }
    return QObject::disconnect(senderObj, sig, receiver, slt);
}
} // namespace Ioc
} // namespace Mc
