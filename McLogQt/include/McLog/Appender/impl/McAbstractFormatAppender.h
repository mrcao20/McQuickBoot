#pragma once

#include "McAbstractAppender.h"

MC_FORWARD_DECL_CLASS(IMcLayout);

MC_FORWARD_DECL_PRIVATE_DATA(McAbstractFormatAppender);

class MCLOGQT_EXPORT McAbstractFormatAppender
        : public McAbstractAppender
{
    Q_OBJECT
    MC_DECL_INIT(McAbstractFormatAppender)
    MC_DEFINE_TYPELIST(MC_DECL_TYPELIST(McAbstractAppender))
    Q_PROPERTY(IMcLayoutPtr layout READ layout WRITE setLayout)
public:
    McAbstractFormatAppender();
    ~McAbstractFormatAppender() override;
    
    IMcLayoutPtr layout() const noexcept;
    void setLayout(IMcLayoutConstPtrRef val) noexcept;
    
    void finished() noexcept override;
    void threadFinished() noexcept override;
    
protected:
    void doAppend(QtMsgType type, const QMessageLogContext &context, const QString &str) noexcept override;
    
    virtual void writeBefore() noexcept = 0;
    virtual void writeAfter() noexcept = 0;
    
private:
    MC_DECL_PRIVATE(McAbstractFormatAppender)
};

MC_DECL_METATYPE(McAbstractFormatAppender)
