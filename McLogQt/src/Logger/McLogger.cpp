#include "McLog/Logger/impl/McLogger.h"

#include "McLog/Appender/IMcConfigurableAppender.h"

MC_DECL_PRIVATE_DATA(McLogger)
QString threshold;
QList<IMcConfigurableAppenderPtr> appenders;
MC_DECL_PRIVATE_DATA_END

MC_INIT(McLogger)
MC_REGISTER_BEAN_FACTORY(MC_TYPELIST(McLogger))
MC_REGISTER_LIST_CONVERTER(QList<IMcConfigurableAppenderPtr>)
MC_INIT_END

McLogger::McLogger() 
{
    MC_NEW_PRIVATE_DATA(McLogger);
}

McLogger::~McLogger() 
{
}

QString McLogger::threshold() const noexcept 
{
    return d->threshold;
}

void McLogger::setThreshold(const QString &val) noexcept 
{
    d->threshold = val;
}

QList<IMcConfigurableAppenderPtr> McLogger::appenders() const noexcept 
{
    return d->appenders;
}

void McLogger::setAppenders(const QList<IMcConfigurableAppenderPtr> &val) noexcept 
{
    d->appenders = val;
}

void McLogger::finished() noexcept 
{
    if(threshold().isEmpty())
        setThreshold("debug-");
    for(auto appender : d->appenders) {
        if(appender->threshold().isEmpty()) {
            appender->setThreshold(threshold());
        }
    }
}

void McLogger::log(QtMsgType type, const QMessageLogContext &context, const QString &str) noexcept 
{
    for(auto appender : d->appenders) {
        appender->append(type, context, str);
    }
}
