#pragma once

#include "../McLogGlobal.h"

QT_BEGIN_NAMESPACE
MC_FORWARD_DECL_CLASS(QIODevice);
QT_END_NAMESPACE

class IMcWritableAppender 
{
    MC_DEFINE_TYPELIST()
public:
    virtual ~IMcWritableAppender() = default;
    
    virtual void setDevice(QIODeviceConstPtrRef device) noexcept = 0;
};

MC_DECL_METATYPE(IMcWritableAppender)
