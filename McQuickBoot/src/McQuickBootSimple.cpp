#include "McBoot/McQuickBootSimple.h"

#include <QDebug>

#include <McIoc/ApplicationContext/impl/McAnnotationApplicationContext.h>
#include <McIoc/BeanDefinition/IMcBeanDefinition.h>

namespace {

struct McQuickBootSimpleStaticData
{
    McQuickBootSimplePtr boot;
};

}

MC_GLOBAL_STATIC(McQuickBootSimpleStaticData, mcQuickBootSimpleStaticData)

MC_DECL_PRIVATE_DATA(McQuickBootSimple)
McAnnotationApplicationContextPtr context;
MC_DECL_PRIVATE_DATA_END

MC_INIT(McQuickBootSimple)
MC_DESTROY()
if (!mcQuickBootSimpleStaticData.exists()) {
    return;
}
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
    McAbstractQuickBoot::setInstance(mcQuickBootSimpleStaticData->boot);
    McQuickBootSimplePtr &boot = mcQuickBootSimpleStaticData->boot;
    if(boot->d->context.isNull()) {
        boot->d->context = McAnnotationApplicationContextPtr::create();
        boot->doRefresh();
    }
}

QSharedPointer<McQuickBootSimple> McQuickBootSimple::instance() noexcept
{
    McQuickBootSimplePtr &boot = mcQuickBootSimpleStaticData->boot;
    Q_ASSERT_X(!boot.isNull(), "McQuickBootSimplePtr::instance()", "please call init before");
    return boot;
}

void McQuickBootSimple::refresh() noexcept
{
    d->context->generateReader();
    doRefresh();
}

IMcApplicationContextPtr McQuickBootSimple::getApplicationContext() const noexcept 
{
    return d->context;
}

void McQuickBootSimple::initContainer() const noexcept
{
}
