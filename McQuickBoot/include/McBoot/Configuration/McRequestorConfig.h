#pragma once

#include "../McBootGlobal.h"

MC_FORWARD_DECL_PRIVATE_DATA(McRequestorConfig);

class McRequestorConfig : public QObject
{
    Q_OBJECT
    MC_DECL_INIT(McRequestorConfig)
    MC_COMPONENT
    MC_BEANNAME("requestorConfig")
    MC_CONFIGURATION_PROPERTIES("boot.application.requestor")
    Q_PROPERTY(int maxThreadCount READ maxThreadCount WRITE setMaxThreadCount)
public:
    Q_INVOKABLE McRequestorConfig(QObject *parent = nullptr) noexcept;
    ~McRequestorConfig();

    int maxThreadCount() const noexcept;
    void setMaxThreadCount(int val) noexcept;

private:
    MC_DECL_PRIVATE(McRequestorConfig)
};

MC_DECL_METATYPE(McRequestorConfig)
