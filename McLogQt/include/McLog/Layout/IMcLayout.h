#pragma once

#include "../McLogGlobal.h"

class IMcLayout 
{
public:
    virtual ~IMcLayout() = default;
    
    virtual QString format(QtMsgType type, const QMessageLogContext &context, const QString &str) noexcept = 0;
};

MC_DECL_METATYPE(IMcLayout)
