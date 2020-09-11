#pragma once

#include "../McLogGlobal.h"

class IMcLayout 
{
    MC_DEFINE_TYPELIST()
public:
    virtual ~IMcLayout() = default;
    
    virtual QString format(QtMsgType type, const QMessageLogContext &context, const QString &str) noexcept = 0;
};

MC_DECL_METATYPE(IMcLayout)
