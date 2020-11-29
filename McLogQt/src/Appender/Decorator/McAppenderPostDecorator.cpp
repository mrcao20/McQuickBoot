#include "McLog/Appender/Decorator/McAppenderPostDecorator.h"

MC_INIT(McAppenderPostDecorator)
MC_REGISTER_BEAN_FACTORY(MC_TYPELIST(McAppenderPostDecorator))
MC_INIT_END

MC_DECL_PRIVATE_DATA(McAppenderPostDecorator)
MC_DECL_PRIVATE_DATA_END

McAppenderPostDecorator::McAppenderPostDecorator(QObject *parent)
    : McAbstractAppenderDecorator(parent)
{
    MC_NEW_PRIVATE_DATA(McAppenderPostDecorator);
}

McAppenderPostDecorator::~McAppenderPostDecorator()
{
}

void McAppenderPostDecorator::append(QtMsgType type, const QMessageLogContext &context, const QString &str) noexcept
{
    for(auto appender : appenders()) {
        appender->append(type, context, str);
        appender->append(type, context, separator());
    }
}
