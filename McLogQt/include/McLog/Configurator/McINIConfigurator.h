#pragma once

#include "McSettingConfigurator.h"

class MCLOGQT_EXPORT McINIConfigurator : public McSettingConfigurator 
{
    Q_OBJECT
    using McSettingConfigurator::configure;
public:
    McINIConfigurator();
    
    static void configure(const QString &path) noexcept;
};
