#pragma once

#include "../McLogGlobal.h"

QT_BEGIN_NAMESPACE
class QTextCodec;
QT_END_NAMESPACE

class IMcCodecableAppender
{
public:
    virtual ~IMcCodecableAppender() = default;

    virtual QTextCodec *codec() const noexcept = 0;
    virtual void setCodec(QTextCodec *val) noexcept = 0;
};

MC_DECL_METATYPE(IMcCodecableAppender)
