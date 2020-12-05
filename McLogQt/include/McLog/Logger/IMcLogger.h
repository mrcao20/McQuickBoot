#pragma once

#include "../McLogGlobal.h"

class IMcLogger 
{
    MC_DEFINE_TYPELIST()
public:
    virtual ~IMcLogger() = default;
    
    virtual void log(QtMsgType type, const QMessageLogContext &context, const QString &str) noexcept = 0;
};

MC_DECL_METATYPE(IMcLogger)
