#pragma once

#include "McAbstractAppender.h"
#include "../IMcWritableAppender.h"

MC_FORWARD_DECL_PRIVATE_DATA(McAbstractIODeviceAppender);

class McAbstractIODeviceAppender
        : public McAbstractAppender
        , public IMcWritableAppender
{
    Q_OBJECT
    MC_DECL_INIT(McAbstractIODeviceAppender)
    MC_DEFINE_TYPELIST(MC_DECL_TYPELIST(McAbstractAppender), MC_DECL_TYPELIST(IMcWritableAppender));
public:
    McAbstractIODeviceAppender();
    ~McAbstractIODeviceAppender();
    
    QIODevicePtr device() const noexcept;
    void setDevice(QIODeviceConstPtrRef device) noexcept override;
    
private:
    MC_DECL_PRIVATE(McAbstractIODeviceAppender)
};

MC_DECL_METATYPE(McAbstractIODeviceAppender)

