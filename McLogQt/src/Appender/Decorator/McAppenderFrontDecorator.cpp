#include "McLog/Appender/Decorator/McAppenderFrontDecorator.h"

#include "McLog/Appender/Decorator/McAppenderSeparator.h"

MC_INIT(McAppenderFrontDecorator)
MC_REGISTER_BEAN_FACTORY(McAppenderFrontDecorator)
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

void McAppenderFrontDecorator::doAppend(IMcConfigurableAppenderConstPtrRef appender, QtMsgType type, const QMessageLogContext &context, const QString &str) noexcept
{
    writeSeparator(appender, type, context);
    appender->append(type, context, str);
}
