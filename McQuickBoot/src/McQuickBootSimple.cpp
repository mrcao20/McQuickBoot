#include "McBoot/McQuickBootSimple.h"

#include <QDebug>

#include <McIoc/ApplicationContext/impl/McAnnotationApplicationContext.h>
#include <McIoc/BeanDefinition/IMcBeanDefinition.h>

#include "McBoot/BeanDefinitionReader/impl/McConfigurationFileBeanDefinitionReader.h"
#include "McBoot/Configuration/McConfigurationContainer.h"
#include "McBoot/Controller/impl/McControllerContainer.h"
#include "McBoot/Model/McModelContainer.h"

namespace {

struct McQuickBootSimpleStaticData
{
    McQuickBootSimplePtr boot;
};

}

MC_GLOBAL_STATIC(McQuickBootSimpleStaticData, mcQuickBootSimpleStaticData)

MC_DECL_PRIVATE_DATA(McQuickBootSimple)
McAnnotationApplicationContextPtr context;
McCppRequestorPtr requestor;
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

McQuickBootSimple::McQuickBootSimple(QObject *parent) : McAbstractQuickBoot(parent)
{
    MC_NEW_PRIVATE_DATA(McQuickBootSimple);
}

McQuickBootSimple::~McQuickBootSimple() {}

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

    boot->initContainer();

    auto controllerContainer = appCtx->getBean<McControllerContainer>("controllerContainer");
    boot->d->requestor = appCtx->getBean<McCppRequestor>("cppRequestor");
    boot->d->requestor->setControllerContainer(controllerContainer);
}

QSharedPointer<McQuickBootSimple> McQuickBootSimple::instance() noexcept
{
    McQuickBootSimplePtr &boot = mcQuickBootSimpleStaticData->boot;
    Q_ASSERT_X(!boot.isNull(), "McQuickBootSimplePtr::instance()", "please call init before");
    return boot;
}

McCppRequestor &McQuickBootSimple::requestor() const noexcept
{
    return *d->requestor.data();
}

void McQuickBootSimple::refresh() const noexcept
{
    d->context->generateReader();
    auto reader = McConfigurationFileBeanDefinitionReaderPtr::create(d->context);
    reader->readBeanDefinition(d->context.data());
    d->context->refresh();
    initContainer();
}

IMcApplicationContextPtr McQuickBootSimple::getApplicationContext() const noexcept 
{
    return d->context;
}

void McQuickBootSimple::initContainer() const noexcept
{
    auto controllerContainer = d->context->getBean<McControllerContainer>("controllerContainer");
    controllerContainer->init(this);

    auto modelContainer = d->context->getBean<McModelContainer>("modelContainer");
    modelContainer->init(this);

    auto configurationContainer = d->context->getBean<McConfigurationContainer>(
        "configurationContainer");
    configurationContainer->init(this);
}
