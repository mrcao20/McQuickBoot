#include "McBoot/McQuickBootSimple.h"

#include <QDebug>

#include <McIoc/ApplicationContext/impl/McAnnotationApplicationContext.h>
#include <McIoc/BeanDefinition/IMcBeanDefinition.h>

#include "McBoot/Controller/impl/McControllerContainer.h"
#include "McBoot/Model/McModelContainer.h"
#include "McBoot/Socket/impl/McQmlSocketContainer.h"
#include "McBoot/BeanDefinitionReader/impl/McConfigurationFileBeanDefinitionReader.h"

namespace {

struct McQuickBootSimpleStaticData
{
    McQuickBootSimplePtr boot;
};

}

MC_GLOBAL_STATIC(McQuickBootSimpleStaticData, mcQuickBootSimpleStaticData)

MC_DECL_PRIVATE_DATA(McQuickBootSimple)
McAnnotationApplicationContextPtr context;
McQmlRequestorPtr requestor;
MC_DECL_PRIVATE_DATA_END

MC_INIT(McQuickBootSimple)
MC_DESTROY()
if(!mcQuickBootSimpleStaticData.exists()) {
    return;
}
auto requestor = mcQuickBootSimpleStaticData->boot->d->requestor.data();
mcQuickBootSimpleStaticData->boot->d->requestor.clear();
delete requestor;
mcQuickBootSimpleStaticData->boot.reset();
MC_INIT_END

McQuickBootSimple::McQuickBootSimple(QObject *parent)
    : QObject(parent)
{
    MC_NEW_PRIVATE_DATA(McQuickBootSimple)
}

McQuickBootSimple::~McQuickBootSimple()
{
    qDebug() << "~McQuickBootSimple";
}

void McQuickBootSimple::init() noexcept
{
    if(!mcQuickBootSimpleStaticData->boot.isNull()) {
        qWarning("the boot is already init.");
        return;
    }
    mcQuickBootSimpleStaticData->boot = McQuickBootSimplePtr::create();
    McQuickBootSimplePtr &boot = mcQuickBootSimpleStaticData->boot;
    if(boot->d->context.isNull()) {
        boot->d->context = McAnnotationApplicationContextPtr::create();
        auto reader = McConfigurationFileBeanDefinitionReaderPtr::create(boot->d->context);
        reader->readBeanDefinition(boot->d->context.data());
        boot->d->context->refresh();  //!< 预加载bean
    }
    auto appCtx = boot->d->context;
    
    auto controllerContainer = appCtx->getBean<McControllerContainer>("controllerContainer");
    controllerContainer->init(boot);
    
    auto modelContainer = appCtx->getBean<McModelContainer>("modelContainer");
    modelContainer->init(boot);
    
    auto socketContainer = appCtx->getBean<McQmlSocketContainer>("socketContainer");
    socketContainer->init(boot);
    
    boot->d->requestor = appCtx->getBean<McQmlRequestor>("requestor");
    boot->d->requestor->setControllerContainer(controllerContainer);
    boot->d->requestor->setSocketContainer(socketContainer);
}

McQmlRequestorPtr McQuickBootSimple::requestor() noexcept
{
    McQuickBootSimplePtr &boot = mcQuickBootSimpleStaticData->boot;
    if(boot.isNull()) {
        return McQmlRequestorPtr();
    }
    return boot->d->requestor;
}

IMcApplicationContextPtr McQuickBootSimple::getApplicationContext() const noexcept 
{
    return d->context;
}
