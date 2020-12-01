#pragma once

#include "../IMcConfigurableAppender.h"

MC_FORWARD_DECL_PRIVATE_DATA(McAbstractAppender);

class MCLOGQT_EXPORT McAbstractAppender 
        : public QObject
        , public IMcConfigurableAppender
        
{
    Q_OBJECT
    MC_DECL_INIT(McAbstractAppender)
    MC_DEFINE_TYPELIST(QObject, MC_DECL_TYPELIST(IMcConfigurableAppender))
    Q_PROPERTY(QString threshold READ threshold WRITE setThreshold)
public:
    McAbstractAppender();
    ~McAbstractAppender() override;
    
    QString threshold() const noexcept override;
    void setThreshold(const QString &val) noexcept override;
    
    QList<QtMsgType> types() const noexcept override;
    
    virtual
    Q_INVOKABLE
    MC_BEAN_FINISHED
    void finished() noexcept;
    
    virtual
    Q_INVOKABLE
    MC_THREAD_FINISHED
    void threadFinished() noexcept;
    
private:
    QList<QtMsgType> initThreshold(const QString &val) const noexcept;
    int strToEnum(const QString &val) const noexcept;
    
private:
    MC_DECL_PRIVATE(McAbstractAppender)
};

MC_DECL_METATYPE(McAbstractAppender)
