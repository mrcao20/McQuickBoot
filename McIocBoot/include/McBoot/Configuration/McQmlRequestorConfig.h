#pragma once

#include "../McBootGlobal.h"

MC_FORWARD_DECL_PRIVATE_DATA(McQmlRequestorConfig);

class McQmlRequestorConfig : public QObject
{
    Q_OBJECT
    MC_DECL_INIT(McQmlRequestorConfig)
    MC_COMPONENT
    MC_BEANNAME("requestorConfig")
    MC_CONFIGURATION_PROPERTIES("boot.application.requestor")
    Q_PROPERTY(int maxThreadCount READ maxThreadCount WRITE setMaxThreadCount)
public:
    Q_INVOKABLE McQmlRequestorConfig(QObject *parent = nullptr) noexcept;
    ~McQmlRequestorConfig();
    
    int maxThreadCount() const noexcept;
    void setMaxThreadCount(int val) noexcept;

private:
    MC_DECL_PRIVATE(McQmlRequestorConfig)
};

MC_DECL_METATYPE(McQmlRequestorConfig)
