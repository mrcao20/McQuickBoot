#include "McLog/Appender/Decorator/McAppenderPostDecorator.h"

#include "McLog/Appender/Decorator/McAppenderSeparator.h"

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

void McAppenderPostDecorator::doAppend(IMcConfigurableAppenderConstPtrRef appender, QtMsgType type, const QMessageLogContext &context, const QString &str) noexcept
{
    appender->append(type, context, str);
    writeSeparator(appender, type, context);
}
