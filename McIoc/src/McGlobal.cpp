#include "McIoc/McGlobal.h"

#include <QEventLoop>
#include <QTimer>
#include <QElapsedTimer>
#include <QDir>
#include <QCoreApplication>
#include <QUrl>
#include <QMetaObject>
#include <QMetaClassInfo>
#include <QDebug>

#include "McIoc/BeanDefinition/IMcBeanDefinition.h"
#include "McIoc/ApplicationContext/IMcApplicationContext.h"

namespace McPrivate {

MC_INIT(McGlobal)
auto pId = qRegisterMetaType<QObject *>("QObject");
auto sId = qRegisterMetaType<QObjectPtr>(MC_MACRO_STR(QObjectConstPtrRef));
McMetaTypeId::addQObjectPointerIds(pId, sId);
McMetaTypeId::addSharedPointerId(sId, pId);
MC_INIT_END

}

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

}
