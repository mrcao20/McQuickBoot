#pragma once

#include "../McBootGlobal.h"

MC_FORWARD_DECL_PRIVATE_DATA(McLogConfig);

class McLogConfig : public QObject
{
    Q_OBJECT
    MC_COMPONENT("logConfig")
    MC_CONFIGURATION_PROPERTIES("boot.application.log")
    Q_PRIVATE_PROPERTY(d, QStringList xmlPaths MEMBER xmlPaths)
    Q_PRIVATE_PROPERTY(d, QString flag MEMBER flag)
public:
    explicit McLogConfig(QObject *parent = nullptr) noexcept;
    ~McLogConfig() override;

private:
    Q_INVOKABLE
    MC_BEAN_FINISHED
    void allFinished() noexcept;

private:
    MC_DECL_PRIVATE(McLogConfig)
};

MC_DECL_METATYPE(McLogConfig)
