#include "McBoot/McQuickBoot.h"

#include <QQmlContext>
#include <QJSValue>
#include <QGlobalStatic>
#include <QQuickView>
#include <QDebug>

#include <McIoc/ApplicationContext/impl/McAnnotationApplicationContext.h>
#include <McIoc/BeanDefinition/IMcBeanDefinition.h>

#include "McBoot/Controller/impl/McControllerContainer.h"
#include "McBoot/Model/McModelContainer.h"
#include "McBoot/Socket/impl/McQmlSocketContainer.h"
#include "McBoot/BeanDefinitionReader/impl/McConfigurationFileBeanDefinitionReader.h"

namespace {

struct McQuickBootStaticData
{
    McQuickBootPtr boot;
    function<void(QCoreApplication *)> preInitFunc;
    function<void(QCoreApplication *, QQmlApplicationEngine *)> afterInitFunc;
};

}

Q_GLOBAL_STATIC(McQuickBootStaticData, mcQuickBootStaticData)

MC_DECL_PRIVATE_DATA(McQuickBoot)
McAnnotationApplicationContextPtr context;
QQmlEngine *engine{nullptr};
McQmlRequestorPtr requestor;
MC_DECL_PRIVATE_DATA_END

MC_INIT(McQuickBoot)
MC_DESTROY()
if(!mcQuickBootStaticData.exists()) {
    return;
}
auto requestor = mcQuickBootStaticData->boot->d->requestor.data();
mcQuickBootStaticData->boot->d->requestor.clear();
delete requestor;
mcQuickBootStaticData->boot.reset();
MC_INIT_END

McQuickBoot::McQuickBoot(QObject *parent)
    : QObject(parent)
{
    MC_NEW_PRIVATE_DATA(McQuickBoot)
}

McQuickBoot::~McQuickBoot()
{
    qDebug() << "~McQuickBoot";
}

void McQuickBoot::init(QCoreApplication *app, QQmlApplicationEngine *engine) noexcept
{
    if(mcQuickBootStaticData->preInitFunc) {
        mcQuickBootStaticData->preInitFunc(app);
    }
    
    if(!mcQuickBootStaticData->boot.isNull()) {
        qWarning("the boot is already init.");
        return;
    }
    mcQuickBootStaticData->boot = McQuickBootPtr::create();
    McQuickBootPtr &boot = mcQuickBootStaticData->boot;
    boot->initBoot(engine);
    auto appCtx = boot->d->context;
    
    auto controllerContainer = appCtx->getBean<McControllerContainer>("controllerContainer");
    controllerContainer->init(boot);
    
    auto modelContainer = appCtx->getBean<McModelContainer>("modelContainer");
    modelContainer->init(boot);
    
    auto socketContainer = appCtx->getBean<McQmlSocketContainer>("socketContainer");
    socketContainer->init(boot);
    
    auto requestor = appCtx->getBean<McQmlRequestor>("requestor");
    requestor->setControllerContainer(controllerContainer);
    requestor->setSocketContainer(socketContainer);
    
    boot->d->requestor = appCtx->getBean<McQmlRequestor>("requestor");
    boot->d->requestor->setControllerContainer(controllerContainer);
    boot->d->requestor->setSocketContainer(socketContainer);
    
    //! engine的newQObject函数会将其参数所有权转移到其返回的QJSValue中
    QJSValue jsObj = engine->newQObject(requestor.data());
    engine->globalObject().setProperty("$", jsObj);
    engine->importModule(":/requestor.js.inl");
    
    if(mcQuickBootStaticData->afterInitFunc) {
        mcQuickBootStaticData->afterInitFunc(app, engine);
    }
}

QQmlEngine *McQuickBoot::engine() noexcept
{
    return mcQuickBootStaticData->boot->d->engine;
}

QQuickView *McQuickBoot::createQuickView(const QString &source, QWindow *parent) noexcept
{
    if(McQuickBoot::engine() == nullptr) {
        qCritical("engine is null. you must be call function init or run before\n");
        return nullptr;
    }
    QQuickView *view = new QQuickView(engine(), parent);
    view->setSource(QUrl(Mc::toAbsolutePath(source)));
    return  view;
}

void McQuickBoot::setPreInitFunc(const function<void(QCoreApplication *)> &func) noexcept
{
    mcQuickBootStaticData->preInitFunc = func;
}

void McQuickBoot::setAfterInitFunc(const function<void(QCoreApplication *, QQmlApplicationEngine *)> &func) noexcept
{
    mcQuickBootStaticData->afterInitFunc = func;
}

McQmlRequestorPtr McQuickBoot::requestor() noexcept
{
    McQuickBootPtr &boot = mcQuickBootStaticData->boot;
    if(boot.isNull()) {
        return McQmlRequestorPtr();
    }
    return boot->d->requestor;
}

void McQuickBoot::initBoot(QQmlEngine *engine) noexcept 
{
    if (d->context) {
		qInfo() << "The container has been initialized";
		return;
	}
    d->engine = engine;
    d->context = McAnnotationApplicationContextPtr::create();
#ifndef MC_NO_YAML
    auto reader = McConfigurationFileBeanDefinitionReaderPtr::create(d->context);
    reader->readBeanDefinition(d->context.data());
#endif
    d->context->refresh();  //!< 预加载bean
}

IMcApplicationContextPtr McQuickBoot::getApplicationContext() const noexcept 
{
    return d->context;
}

QList<QString> McQuickBoot::getAllComponent() noexcept
{
    return Mc::getAllComponent(getApplicationContext());
}

QList<QString> McQuickBoot::getComponents(const QString &componentType) noexcept 
{
    return Mc::getComponents(getApplicationContext(), componentType);
}
