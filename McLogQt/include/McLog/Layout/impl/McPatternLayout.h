#pragma once

#include "../IMcLayout.h"

MC_FORWARD_DECL_PRIVATE_DATA(McPatternLayout);

class MCLOGQT_EXPORT McPatternLayout : public QObject, public IMcLayout 
{
    Q_OBJECT
    MC_DECL_INIT(McPatternLayout)
    MC_TYPELIST(QObject, IMcLayout)
    Q_PROPERTY(QString pattern READ getPattern WRITE setPattern)
public:
    Q_INVOKABLE McPatternLayout();
    ~McPatternLayout() override;
    
    QString getPattern() const noexcept;
    void setPattern(const QString &val) noexcept;
    
    QString format(QtMsgType type, const QMessageLogContext &context, const QString &str) noexcept override;
    
    virtual
    Q_INVOKABLE
    MC_BEAN_FINISHED
    void finished() noexcept;
private:
    MC_DECL_PRIVATE(McPatternLayout)
};

MC_DECL_METATYPE(McPatternLayout)
