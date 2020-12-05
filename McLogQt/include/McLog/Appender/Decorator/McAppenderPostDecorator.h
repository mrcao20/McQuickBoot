#pragma once

#include "McAbstractAppenderDecorator.h"

MC_FORWARD_DECL_PRIVATE_DATA(McAppenderPostDecorator);

class MCLOGQT_EXPORT McAppenderPostDecorator
        : public McAbstractAppenderDecorator
{
    Q_OBJECT
    MC_DECL_INIT(McAppenderPostDecorator)
    MC_DEFINE_TYPELIST(MC_DECL_TYPELIST(McAbstractAppenderDecorator));
public:
    Q_INVOKABLE explicit McAppenderPostDecorator(QObject *parent = nullptr);
    ~McAppenderPostDecorator() override;
    
protected:
    void doAppend(IMcConfigurableAppenderConstPtrRef appender, QtMsgType type, const QMessageLogContext &context, const QString &str) noexcept override;
    
private:
    MC_DECL_PRIVATE(McAppenderPostDecorator)
};

MC_DECL_METATYPE(McAppenderPostDecorator)

