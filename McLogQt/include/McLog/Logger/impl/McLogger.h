#pragma once

#include "../IMcLogger.h"

#include <QList>

MC_FORWARD_DECL_CLASS(IMcConfigurableAppender)

MC_FORWARD_DECL_PRIVATE_DATA(McLogger);

class MCLOGQT_EXPORT McLogger : public QObject, public IMcLogger 
{
    Q_OBJECT
    MC_DECL_INIT(McLogger)
    MC_TYPELIST(QObject, IMcLogger)
    Q_PROPERTY(QString threshold READ threshold WRITE setThreshold)
    Q_PROPERTY(QList<IMcConfigurableAppenderPtr> appenders READ appenders WRITE setAppenders)
public:
    Q_INVOKABLE McLogger();
    ~McLogger() override;
    
    QString threshold() const noexcept;
    void setThreshold(const QString &val) noexcept;
    
    QList<IMcConfigurableAppenderPtr> appenders() const noexcept;
    void setAppenders(const QList<IMcConfigurableAppenderPtr> &val) noexcept;
    
    virtual
    Q_INVOKABLE
    MC_BEAN_FINISHED
    void finished() noexcept;
    
    void log(QtMsgType type, const QMessageLogContext &context, const QString &str) noexcept override;

private:
    MC_DECL_PRIVATE(McLogger)
};

MC_DECL_METATYPE(McLogger)
