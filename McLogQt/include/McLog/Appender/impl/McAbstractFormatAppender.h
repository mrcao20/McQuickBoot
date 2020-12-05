#pragma once

#include "McAbstractIODeviceAppender.h"

MC_FORWARD_DECL_CLASS(IMcLayout);

MC_FORWARD_DECL_PRIVATE_DATA(McAbstractFormatAppender);

class MCLOGQT_EXPORT McAbstractFormatAppender
        : public McAbstractIODeviceAppender
{
    Q_OBJECT
    MC_DECL_INIT(McAbstractFormatAppender)
    MC_DEFINE_TYPELIST(MC_DECL_TYPELIST(McAbstractIODeviceAppender))
    Q_PROPERTY(IMcLayoutPtr layout READ layout WRITE setLayout)
    Q_PROPERTY(bool immediateFlush READ immediateFlush WRITE setImmediateFlush)
public:
    McAbstractFormatAppender();
    ~McAbstractFormatAppender() override;
    
    IMcLayoutPtr layout() const noexcept;
    void setLayout(IMcLayoutConstPtrRef val) noexcept;
    
    bool immediateFlush() const noexcept;
    void setImmediateFlush(bool val) noexcept;
    
    void append(QtMsgType type, const QMessageLogContext &context, const QString &str) noexcept override;
    
    void finished() noexcept override;
    void threadFinished() noexcept override;
    
protected:
    void customEvent(QEvent *event) override;
    
    virtual void writeBefore() noexcept = 0;
    virtual void writeAfter() noexcept = 0;
    
private:
    Q_INVOKABLE void append_helper(const QString &msg) noexcept;
    
private:
    MC_DECL_PRIVATE(McAbstractFormatAppender)
};

MC_DECL_METATYPE(McAbstractFormatAppender)
