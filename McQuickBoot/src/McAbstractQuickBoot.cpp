#include "McBoot/McAbstractQuickBoot.h"

#include <QScxmlStateMachine>
#include <QThread>

#include <McIoc/ApplicationContext/IMcApplicationContext.h>

#include "McBoot/BeanDefinitionReader/impl/McConfigurationFileBeanDefinitionReader.h"
#include "McBoot/Configuration/McConfigurationContainer.h"
#include "McBoot/Controller/impl/McControllerContainer.h"

MC_GLOBAL_STATIC_BEGIN(mcAbstractQuickBootStaticData)
McAbstractQuickBootPtr boot;
MC_GLOBAL_STATIC_END(mcAbstractQuickBootStaticData)

MC_DECL_PRIVATE_DATA(McAbstractQuickBoot)
McCppRequestorPtr requestor;
QThread *workThread{nullptr};
MC_DECL_PRIVATE_DATA_END

MC_INIT(McAbstractQuickBoot)
qRegisterMetaType<QThread *>();
MC_DESTROY()
if (!mcAbstractQuickBootStaticData.exists()) {
    return;
}
if (mcAbstractQuickBootStaticData->boot->d->workThread != nullptr) {
    mcAbstractQuickBootStaticData->boot->d->workThread->quit();
    mcAbstractQuickBootStaticData->boot->d->workThread->wait();
    delete mcAbstractQuickBootStaticData->boot->d->workThread;
    mcAbstractQuickBootStaticData->boot->d->workThread = nullptr;
}
mcAbstractQuickBootStaticData->boot.reset();
MC_INIT_END

McAbstractQuickBoot::McAbstractQuickBoot(QObject *parent) : QObject(parent)
{
    MC_NEW_PRIVATE_DATA(McAbstractQuickBoot);
}

McAbstractQuickBoot::~McAbstractQuickBoot() {}

QMetaObject::Connection McAbstractQuickBoot::connect(const QString &sender,
                                                     const QString &signal,
                                                     const QString &receiver,
                                                     const QString &slot,
                                                     Qt::ConnectionType type) noexcept
{
    return Mc::Ioc::connect(McAbstractQuickBoot::instance()->getApplicationContext().data(),
                            sender,
                            signal,
                            receiver,
                            slot,
                            type);
}

QMetaObject::Connection McAbstractQuickBoot::connect(const QString &sender,
                                                     const QString &signal,
                                                     QObject *receiver,
                                                     const QString &slot,
                                                     Qt::ConnectionType type) noexcept
{
    return Mc::Ioc::connect(McAbstractQuickBoot::instance()->getApplicationContext().data(),
                            sender,
                            signal,
                            receiver,
                            slot,
                            type);
}

bool McAbstractQuickBoot::disconnect(const QString &sender,
                                     const QString &signal,
                                     const QString &receiver,
                                     const QString &slot) noexcept
{
    return Mc::Ioc::disconnect(McAbstractQuickBoot::instance()->getApplicationContext().data(),
                               sender,
                               signal,
                               receiver,
                               slot);
}

bool McAbstractQuickBoot::disconnect(const QString &sender,
                                     const QString &signal,
                                     QObject *receiver,
                                     const QString &slot) noexcept
{
    return Mc::Ioc::disconnect(McAbstractQuickBoot::instance()->getApplicationContext().data(),
                               sender,
                               signal,
                               receiver,
                               slot);
}

QSharedPointer<McAbstractQuickBoot> McAbstractQuickBoot::instance() noexcept
{
    McAbstractQuickBootPtr &boot = mcAbstractQuickBootStaticData->boot;
    Q_ASSERT_X(!boot.isNull(), "McAbstractQuickBoot::instance()", "please call init before");
    return boot;
}

void McAbstractQuickBoot::addConfigPath(const QString &path) noexcept
{
    McConfigurationFileBeanDefinitionReader::addConfigPath(path);
}

McCppRequestor &McAbstractQuickBoot::requestor() const noexcept
{
    return *d->requestor.data();
}

void McAbstractQuickBoot::setInstance(const QSharedPointer<McAbstractQuickBoot> &ins) noexcept
{
    mcAbstractQuickBootStaticData->boot = ins;
}

void McAbstractQuickBoot::doRefresh(const QStringList &preloadBeans) noexcept
{
    auto context = getApplicationContext();
    auto reader = McConfigurationFileBeanDefinitionReaderPtr::create(context);
    reader->readBeanDefinition(context.data());
    auto configurationContainer = context->getBean<McConfigurationContainer>(
        "configurationContainer");
    configurationContainer->init(this);
    auto workThreadName = QT_STRINGIFY(MC_QUICKBOOT_WORK_THREAD_NAME);
    if (context->containsBean(workThreadName)) {
        d->workThread = context->getBeanPointer<QThread *>(workThreadName);
    }
    if (d->workThread != nullptr && !d->workThread->isRunning()) {
        d->workThread->start();
    }
    for (auto preloadBean : preloadBeans) {
        context->getBean(preloadBean, d->workThread);
    }
    auto stateMachineName = QT_STRINGIFY(MC_QUICKBOOT_STATE_MACHINE_NAME);
    if (context->containsBean(stateMachineName)) {
        auto val = context->getBeanPointer<QScxmlStateMachine *>(stateMachineName);
        McAbstractRequestor::setStaticStateMachine(val);
    }
    d->requestor = context->getBean<McCppRequestor>("cppRequestor", d->workThread);
    context->refresh(d->workThread); //!< 预加载bean
    initContainer();

    auto controllerContainer = context->getBean<McControllerContainer>("controllerContainer");
    d->requestor->setControllerContainer(controllerContainer);
    d->requestor->setAppCtx(context.data());
}
