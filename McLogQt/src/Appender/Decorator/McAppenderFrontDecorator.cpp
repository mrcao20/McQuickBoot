#include "McLog/Appender/Decorator/McAppenderFrontDecorator.h"

MC_INIT(McAppenderFrontDecorator)
MC_REGISTER_BEAN_FACTORY(MC_TYPELIST(McAppenderFrontDecorator))
MC_INIT_END

MC_DECL_PRIVATE_DATA(McAppenderFrontDecorator)
MC_DECL_PRIVATE_DATA_END

McAppenderFrontDecorator::McAppenderFrontDecorator(QObject *parent)
    : McAbstractAppenderDecorator(parent)
{
    MC_NEW_PRIVATE_DATA(McAppenderFrontDecorator);
}

McAppenderFrontDecorator::~McAppenderFrontDecorator()
{
}

void McAppenderFrontDecorator::append(QtMsgType type, const QMessageLogContext &context, const QString &str) noexcept
{
    for(auto appender : appenders()) {
        appender->append(type, context, separator());
        appender->append(type, context, str);
    }
}
