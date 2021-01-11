#pragma once

#include <QObject>
#include "../IMcConfigurableAppender.h"

#include <QList>

MC_FORWARD_DECL_STRUCT(McAppenderSeparator);

MC_FORWARD_DECL_PRIVATE_DATA(McAbstractAppenderDecorator);

class MCLOGQT_EXPORT McAbstractAppenderDecorator
        : public QObject
        , public IMcConfigurableAppender
{
    Q_OBJECT
    MC_DECL_INIT(McAbstractAppenderDecorator)
    MC_TYPELIST(IMcConfigurableAppender);
    Q_PROPERTY(McAppenderSeparatorPtr separator READ separator WRITE setSeparator)
    Q_PROPERTY(QList<IMcConfigurableAppenderPtr> appenders READ appenders WRITE setAppenders)
public:
    explicit McAbstractAppenderDecorator(QObject *parent = nullptr);
    ~McAbstractAppenderDecorator() override;
    
    McAppenderSeparatorPtr separator() const noexcept;
    void setSeparator(const McAppenderSeparatorPtr &val) noexcept;
    
    QString threshold() const noexcept override;
    void setThreshold(const QString &val) noexcept override;
    
    QList<QtMsgType> types() const noexcept override;
    
    QList<IMcConfigurableAppenderPtr> appenders() const noexcept;
    void setAppenders(const QList<IMcConfigurableAppenderPtr> &val) noexcept;
    
    void append(QtMsgType type, const QMessageLogContext &context, const QString &str) noexcept override;
    
    virtual
    Q_INVOKABLE
    MC_BEAN_FINISHED
    void finished() noexcept;
    
protected:
    virtual void doAppend(IMcConfigurableAppenderConstPtrRef appender, QtMsgType type, const QMessageLogContext &context, const QString &str) noexcept = 0;
    
    void writeSeparator(IMcConfigurableAppenderConstPtrRef appender, QtMsgType type, const QMessageLogContext &context) noexcept;
    
private:
    MC_DECL_PRIVATE(McAbstractAppenderDecorator)
};

MC_DECL_METATYPE(McAbstractAppenderDecorator)

