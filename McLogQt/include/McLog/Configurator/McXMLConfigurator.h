#pragma once

#include "../McLogMacroGlobal.h"

MC_FORWARD_DECL_CLASS(IMcApplicationContext);

class MCLOGQT_EXPORT McXMLConfigurator
{
public:
    static IMcApplicationContextPtr configure(
        const QString &path, const QString &beanName = "defaultLoggerRepository") noexcept;
    static IMcApplicationContextPtr configure(
        const QStringList &paths, const QString &beanName = "defaultLoggerRepository") noexcept;
    static void configure(IMcApplicationContextConstPtrRef appCtx,
                          const QString &beanName = "defaultLoggerRepository") noexcept;

private:
    IMcApplicationContextPtr doConfigure(const QStringList &paths, const QString &beanName) noexcept;
    void doConfigure(IMcApplicationContextConstPtrRef appCtx, const QString &beanName) noexcept;
};
