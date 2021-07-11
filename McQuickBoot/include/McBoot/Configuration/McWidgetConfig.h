#pragma once

#include "../McBootGlobal.h"

MC_FORWARD_DECL_PRIVATE_DATA(McWidgetConfig);

class McWidgetConfig : public QObject
{
    Q_OBJECT
    MC_COMPONENT("widgetConfig")
    MC_CONFIGURATION_PROPERTIES("boot.application.widget")
    Q_PRIVATE_PROPERTY(d, QStringList xmlPaths MEMBER xmlPaths)
    Q_PRIVATE_PROPERTY(d, QString mainWindowName MEMBER mainWindowName)
    Q_PRIVATE_PROPERTY(d, QString flag MEMBER flag)
    Q_PRIVATE_PROPERTY(d, int destroyPriority MEMBER destroyPriority)
public:
    explicit McWidgetConfig(QObject *parent = nullptr) noexcept;
    ~McWidgetConfig() override;

private:
    Q_INVOKABLE
    MC_BEAN_FINISHED
    void allFinished() noexcept;

private:
    MC_DECL_PRIVATE(McWidgetConfig)
};

MC_DECL_METATYPE(McWidgetConfig)
