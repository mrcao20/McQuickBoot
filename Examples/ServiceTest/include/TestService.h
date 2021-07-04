#pragma once

#include <QObject>

#include <McBoot/McBootGlobal.h>
#include <McBoot/Service/IMcServiceLongLiveThread.h>
#include <McBoot/Service/IMcServiceTimer.h>

MC_FORWARD_DECL_PRIVATE_DATA(TestService);

class TestService : public QObject, public IMcServiceTimer, public IMcServiceLongLiveThread
{
    Q_OBJECT
    MC_TYPELIST(IMcServiceTimer, IMcServiceLongLiveThread)
    MC_SERVICE
public:
    Q_INVOKABLE explicit TestService(QObject *parent = nullptr) noexcept;
    ~TestService() override;

    void timeout() noexcept override;
    int interval() noexcept override;
    void moveFinished() noexcept override;

private:
    MC_DECL_PRIVATE(TestService)
};

MC_DECL_METATYPE(TestService)

