#pragma once

#include "../McLogGlobal.h"

class IMcLogger 
{
public:
    virtual ~IMcLogger() = default;
    
    virtual void log(QtMsgType type, const QMessageLogContext &context, const QString &str) noexcept = 0;
};

MC_DECL_METATYPE(IMcLogger)
