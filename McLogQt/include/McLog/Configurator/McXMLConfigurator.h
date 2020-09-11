#pragma once

#include "../McLogGlobal.h"

class MCLOGQT_EXPORT McXMLConfigurator : public QObject 
{
    Q_OBJECT
public:
    McXMLConfigurator();
    
    static void configure(const QString &path, const QString &beanName = "defaultLoggerRepository") noexcept;
    
private:
    void doConfigure(const QString &path, const QString &beanName) noexcept;
};
