#pragma once

#include "../../McLogGlobal.h"

struct MCLOGQT_EXPORT McAppenderSeparator : public QObject
{
    Q_OBJECT
public:
    Q_INVOKABLE McAppenderSeparator() = default;
    
    MC_POCO_PROPERTY(QString, separator)
    
    MC_POCO_PROPERTY(bool, isAutoSeparate, {true})
    
    //! 单位：ms
    MC_POCO_PROPERTY(quint64, autoSeparateInterval, {300000})
};

MC_DECL_METATYPE(McAppenderSeparator)
