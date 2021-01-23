#pragma once

#include "McAbstractAppender.h"
#include "../IMcWritableAppender.h"

MC_FORWARD_DECL_PRIVATE_DATA(McAbstractIODeviceAppender);

class MCLOGQT_EXPORT McAbstractIODeviceAppender
        : public McAbstractAppender
        , public IMcWritableAppender
{
    Q_OBJECT
    MC_DECL_INIT(McAbstractIODeviceAppender)
    MC_TYPELIST(McAbstractAppender, IMcWritableAppender);

public:
    McAbstractIODeviceAppender();
    ~McAbstractIODeviceAppender();
    
    QIODevicePtr device() const noexcept;
    void setDevice(QIODeviceConstPtrRef device) noexcept override;

    void allFinished() noexcept override;

protected:
    QTextStream &textStream() noexcept;

private:
    MC_DECL_PRIVATE(McAbstractIODeviceAppender)
};

MC_DECL_METATYPE(McAbstractIODeviceAppender)

