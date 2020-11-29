#pragma once

#include "McAbstractAppenderDecorator.h"

MC_FORWARD_DECL_PRIVATE_DATA(McAppenderFrontDecorator);

class MCLOGQT_EXPORT McAppenderFrontDecorator
        : public McAbstractAppenderDecorator
{
    Q_OBJECT
    MC_DECL_INIT(McAppenderFrontDecorator)
    MC_DEFINE_TYPELIST(MC_DECL_TYPELIST(McAbstractAppenderDecorator));
public:
    Q_INVOKABLE explicit McAppenderFrontDecorator(QObject *parent = nullptr);
    ~McAppenderFrontDecorator() override;
    
    void append(QtMsgType type, const QMessageLogContext &context, const QString &str) noexcept override;
    
private:
    MC_DECL_PRIVATE(McAppenderFrontDecorator)
};

MC_DECL_METATYPE(McAppenderFrontDecorator)

