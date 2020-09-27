#pragma once

#include "McSettingApplicationContext.h"

class MCIOCCONTAINER_EXPORT McYamlSettingApplicationContext 
        : public McSettingApplicationContext 
{
    Q_OBJECT
    MC_DECL_INIT(McYamlSettingApplicationContext)
public:
    explicit McYamlSettingApplicationContext(QObject *parent = nullptr);
    McYamlSettingApplicationContext(const QString &location, QObject *parent = nullptr);
    McYamlSettingApplicationContext(const QStringList &locations, QObject *parent = nullptr);
};
        
MC_DECL_POINTER(McYamlSettingApplicationContext)
