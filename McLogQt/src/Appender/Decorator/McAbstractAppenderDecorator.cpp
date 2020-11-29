#include "McLog/Appender/Decorator/McAbstractAppenderDecorator.h"

MC_INIT(McAbstractAppenderDecorator)
MC_REGISTER_BEAN_FACTORY(MC_TYPELIST(McAbstractAppenderDecorator))
MC_REGISTER_LIST_CONVERTER(QList<IMcConfigurableAppenderPtr>)
MC_INIT_END

MC_DECL_PRIVATE_DATA(McAbstractAppenderDecorator)
QString separator;
QString threshold;
QList<IMcConfigurableAppenderPtr> appenders;
MC_DECL_PRIVATE_DATA_END

McAbstractAppenderDecorator::McAbstractAppenderDecorator(QObject *parent)
    : QObject(parent)
{
    MC_NEW_PRIVATE_DATA(McAbstractAppenderDecorator);
}

McAbstractAppenderDecorator::~McAbstractAppenderDecorator()
{
}

QString McAbstractAppenderDecorator::separator() const noexcept
{
    return d->separator;
}

void McAbstractAppenderDecorator::setSeparator(const QString &val) noexcept
{
    d->separator = val;
}

QString McAbstractAppenderDecorator::threshold() const noexcept
{
    return d->threshold;
}

void McAbstractAppenderDecorator::setThreshold(const QString &val) noexcept
{
    d->threshold = val;
    
    for(auto appender : d->appenders) {
        if(appender->threshold().isEmpty()) {
            appender->setThreshold(threshold());
        }
    }
}

QList<IMcConfigurableAppenderPtr> McAbstractAppenderDecorator::appenders() const noexcept
{
    return d->appenders;
}

void McAbstractAppenderDecorator::setAppenders(const QList<IMcConfigurableAppenderPtr> &val) noexcept
{
    d->appenders = val;
}
