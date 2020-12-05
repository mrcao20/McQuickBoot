#pragma once

#include "../McLogGlobal.h"

class IMcAppender 
{
    MC_DEFINE_TYPELIST()
public:
    virtual ~IMcAppender() = default;
    
    virtual void append(QtMsgType type, const QMessageLogContext &context, const QString &str) noexcept = 0;
};

MC_DECL_METATYPE(IMcAppender)
