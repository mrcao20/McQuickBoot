#pragma once

#include "../McLogMacroGlobal.h"

class MCLOGQT_EXPORT McXMLConfigurator : public QObject 
{
    Q_OBJECT
public:
    McXMLConfigurator();
    
    static void configure(const QString &path, const QString &beanName = "defaultLoggerRepository") noexcept;
    static void configure(const QStringList &paths, const QString &beanName = "defaultLoggerRepository") noexcept;
    
private:
    void doConfigure(const QStringList &paths, const QString &beanName) noexcept;
};
