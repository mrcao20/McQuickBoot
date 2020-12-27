#pragma once

#include "../../McGlobal.h"

class MCIOC_EXPORT McBeanReference : public QObject 
{
    Q_OBJECT
public:
    QString getName() const noexcept 
    { return m_name; }
    void setName(const QString &name) noexcept 
    { m_name = name; }

    QString getPluginPath() const noexcept 
    { return m_pluginPath; }
    void setPluginPath(const QString &pluginPath) noexcept 
    { m_pluginPath = pluginPath; }
    
private:
    QString m_name;                        //!< 被引用bean的名称
    QString m_pluginPath;               //!< 被引用的插件路径，这个参数和m_name只能有一个存在
};

MC_DECL_METATYPE(McBeanReference)
