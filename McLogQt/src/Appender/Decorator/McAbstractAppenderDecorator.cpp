#include "McLog/Appender/Decorator/McAbstractAppenderDecorator.h"

#include <QTimer>

#include "McLog/Appender/Decorator/McAppenderSeparator.h"

MC_INIT(McAbstractAppenderDecorator)
MC_REGISTER_BEAN_FACTORY(McAppenderSeparator)
MC_REGISTER_BEAN_FACTORY(McAbstractAppenderDecorator)
MC_REGISTER_LIST_CONVERTER(QList<IMcConfigurableAppenderPtr>)
MC_INIT_END

MC_DECL_PRIVATE_DATA(McAbstractAppenderDecorator)
McAppenderSeparatorPtr separator;
QString threshold;
QList<IMcConfigurableAppenderPtr> appenders;
QTimer autoSeparateTimer;
MC_DECL_PRIVATE_DATA_END

McAbstractAppenderDecorator::McAbstractAppenderDecorator(QObject *parent)
    : QObject(parent)
{
    MC_NEW_PRIVATE_DATA(McAbstractAppenderDecorator);
}

McAbstractAppenderDecorator::~McAbstractAppenderDecorator()
{
}

McAppenderSeparatorPtr McAbstractAppenderDecorator::separator() const noexcept
{
    return d->separator;
}

void McAbstractAppenderDecorator::setSeparator(const McAppenderSeparatorPtr &val) noexcept
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

QList<QtMsgType> McAbstractAppenderDecorator::types() const noexcept
{
    return QList<QtMsgType>();
}

QList<IMcConfigurableAppenderPtr> McAbstractAppenderDecorator::appenders() const noexcept
{
    return d->appenders;
}

void McAbstractAppenderDecorator::setAppenders(const QList<IMcConfigurableAppenderPtr> &val) noexcept
{
    d->appenders = val;
}

void McAbstractAppenderDecorator::append(QtMsgType type, const QMessageLogContext &context, const QString &str) noexcept
{
    for(auto appender : appenders()) {
        doAppend(appender, type, context, str);
    }
}

void McAbstractAppenderDecorator::finished() noexcept
{
    //! 回归到本类线程中执行
    connect(&d->autoSeparateTimer, &QTimer::timeout, this, [this](){
        QMessageLogContext context(QT_MESSAGELOG_FILE, QT_MESSAGELOG_LINE, QT_MESSAGELOG_FUNC, nullptr);
        for(auto appender : appenders()) {
            auto types = appender->types();
            auto type = types.first();      //!< 不可能为空
            for(auto t : types) {
                if(t <= type) {
                    continue;
                }
                type = t;
            }
            writeSeparator(appender, type, context);
        }
    }, Qt::QueuedConnection);
    d->autoSeparateTimer.start(separator()->autoSeparateInterval);
}

void McAbstractAppenderDecorator::writeSeparator(IMcConfigurableAppenderConstPtrRef appender, QtMsgType type, const QMessageLogContext &context) noexcept
{
    appender->append(type, context, separator()->separator);
}
