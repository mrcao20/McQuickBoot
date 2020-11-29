#pragma once

#include <QObject>
#include "../IMcConfigurableAppender.h"

#include <QList>

MC_FORWARD_DECL_PRIVATE_DATA(McAbstractAppenderDecorator);

class MCLOGQT_EXPORT McAbstractAppenderDecorator
        : public QObject
        , public IMcConfigurableAppender
{
    Q_OBJECT
    MC_DECL_INIT(McAbstractAppenderDecorator)
    MC_DEFINE_TYPELIST(MC_DECL_TYPELIST(IMcConfigurableAppender));
    Q_PROPERTY(QString separator READ separator WRITE setSeparator)
    Q_PROPERTY(QList<IMcConfigurableAppenderPtr> appenders READ appenders WRITE setAppenders)
public:
    explicit McAbstractAppenderDecorator(QObject *parent = nullptr);
    ~McAbstractAppenderDecorator() override;
    
    QString separator() const noexcept;
    void setSeparator(const QString &val) noexcept;
    
    QString threshold() const noexcept override;
    void setThreshold(const QString &val) noexcept override;
    
    QList<IMcConfigurableAppenderPtr> appenders() const noexcept;
    void setAppenders(const QList<IMcConfigurableAppenderPtr> &val) noexcept;
    
private:
    MC_DECL_PRIVATE(McAbstractAppenderDecorator)
};

MC_DECL_METATYPE(McAbstractAppenderDecorator)

