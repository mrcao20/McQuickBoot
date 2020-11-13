#pragma once

#include "McAbstractFormatAppender.h"

MC_FORWARD_DECL_PRIVATE_DATA(McVSDebugAppender);

class MCLOGQT_EXPORT McVSDebugAppender : public McAbstractFormatAppender
{
    Q_OBJECT
    MC_DECL_INIT(McVSDebugAppender)
    MC_DEFINE_TYPELIST(MC_DECL_TYPELIST(McAbstractFormatAppender));
public:
    Q_INVOKABLE McVSDebugAppender() noexcept;
    ~McVSDebugAppender() override;
    
    void finished() noexcept override;
    
protected:
    void writeBefore() noexcept override;
    void writeAfter() noexcept override;
    
private:
    MC_DECL_PRIVATE(McVSDebugAppender)
};

MC_DECL_METATYPE(McVSDebugAppender)

