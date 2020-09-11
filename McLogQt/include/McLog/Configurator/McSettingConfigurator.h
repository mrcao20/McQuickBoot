#pragma once

#include "../McLogGlobal.h"

QT_BEGIN_NAMESPACE
class QSettings;
QT_END_NAMESPACE

MC_FORWARD_DECL_CLASS(IMcLogger);
MC_FORWARD_DECL_CLASS(IMcConfigurableAppender);

class MCLOGQT_EXPORT McSettingConfigurator : public QObject 
{
    Q_OBJECT
public:
    McSettingConfigurator();
    
    static void configure(QSettings &settings) noexcept;
    
protected:
    void doConfigure(QSettings &settings) noexcept;
    IMcLoggerPtr configLogger(QSettings &settings) noexcept;
    QList<IMcConfigurableAppenderPtr> configAppenders(QSettings &settings) noexcept;
};
