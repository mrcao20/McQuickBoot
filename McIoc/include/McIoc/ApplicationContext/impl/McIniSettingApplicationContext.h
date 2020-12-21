#pragma once

#include "McSettingApplicationContext.h"

class MCIOC_EXPORT McIniSettingApplicationContext : public McSettingApplicationContext
{
    Q_OBJECT
public:
    explicit McIniSettingApplicationContext(QObject *parent = nullptr);
    McIniSettingApplicationContext(const QString &location, QObject *parent = nullptr);
    McIniSettingApplicationContext(const QStringList &locations, QObject *parent = nullptr);
};
        
MC_DECL_POINTER(McIniSettingApplicationContext)
