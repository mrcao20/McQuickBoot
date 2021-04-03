#pragma once

#include "../IMcCodecableAppender.h"
#include "../IMcWritableAppender.h"
#include "McAbstractAppender.h"

MC_FORWARD_DECL_PRIVATE_DATA(McAbstractIODeviceAppender);

class MCLOGQT_EXPORT McAbstractIODeviceAppender : public McAbstractAppender,
                                                  public IMcWritableAppender,
                                                  public IMcCodecableAppender
{
    Q_OBJECT
    MC_DECL_INIT(McAbstractIODeviceAppender)
    MC_TYPELIST(McAbstractAppender, IMcWritableAppender)
    Q_PROPERTY(QByteArray codecName READ codecName WRITE setCodecName)

public:
    McAbstractIODeviceAppender();
    ~McAbstractIODeviceAppender();
    
    QIODevicePtr device() const noexcept;
    void setDevice(QIODeviceConstPtrRef device) noexcept override;
    QByteArray codecName() const noexcept;
    void setCodecName(const QByteArray &val) noexcept;
    QTextCodec *codec() const noexcept override;
    void setCodec(QTextCodec *val) noexcept override;

protected:
    void doAllFinished() noexcept override;

protected:
    QTextStream &textStream() noexcept;

private:
    MC_DECL_PRIVATE(McAbstractIODeviceAppender)
};

MC_DECL_METATYPE(McAbstractIODeviceAppender)

