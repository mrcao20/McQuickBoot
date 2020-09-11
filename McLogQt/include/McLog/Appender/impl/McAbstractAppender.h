#pragma once

#include "../IMcConfigurableAppender.h"
#include "../IMcWritableAppender.h"

#include <QEvent>

class McAppenderEvent : public QEvent 
{
public:
    static const QEvent::Type eventType;
    
    McAppenderEvent(const QString &msg)
        : QEvent(static_cast<QEvent::Type>(eventType))
        , m_msg(msg)
    {}
    ~McAppenderEvent() override;
    QString msg() const noexcept 
    { return m_msg; }
    
private:
    QString m_msg;
};

MC_FORWARD_DECL_CLASS(IMcLayout);

MC_FORWARD_DECL_PRIVATE_DATA(McAbstractAppender);

class MCLOGQT_EXPORT McAbstractAppender 
        : public QObject
        , public IMcConfigurableAppender
        , public IMcWritableAppender
{
    
    Q_OBJECT
    MC_DECL_INIT(McAbstractAppender)
    MC_DEFINE_TYPELIST(QObject, MC_DECL_TYPELIST(IMcConfigurableAppender), MC_DECL_TYPELIST(IMcWritableAppender))
    Q_PROPERTY(IMcLayoutPtr layout READ layout WRITE setLayout)
    Q_PROPERTY(QString threshold READ threshold WRITE setThreshold)
    Q_PROPERTY(bool immediateFlush READ immediateFlush WRITE setImmediateFlush)
public:
    McAbstractAppender();
    ~McAbstractAppender() override;
    
    IMcLayoutPtr layout() const noexcept;
    void setLayout(IMcLayoutConstPtrRef val) noexcept;
    
    QString threshold() const noexcept override;
    void setThreshold(const QString &val) noexcept override;
    
    bool immediateFlush() const noexcept;
    void setImmediateFlush(bool val) noexcept;
    
    void append(QtMsgType type, const QMessageLogContext &context, const QString &str) noexcept override;
    
    QIODevicePtr device() const noexcept;
    void setDevice(QIODeviceConstPtrRef device) noexcept override;
    
    virtual
    Q_INVOKABLE
    MC_BEAN_FINISHED
    void finished() noexcept;
    
    virtual
    Q_INVOKABLE
    MC_THREAD_FINISHED
    void threadFinished() noexcept;
    
protected:
    virtual void flush() noexcept = 0;
    
    void customEvent(QEvent *event) override;
    
private:
    Q_INVOKABLE void append_helper(const QByteArray &msg) noexcept;
    
private:
    QList<QtMsgType> initThreshold(const QString &val) const noexcept;
    int strToEnum(const QString &val) const noexcept;
    
private:
    MC_DECL_PRIVATE(McAbstractAppender)
};

MC_DECL_METATYPE(McAbstractAppender)
