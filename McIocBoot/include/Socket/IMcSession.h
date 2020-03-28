#pragma once

#include "../McBootGlobal.h"

class IMcSession {
    MC_DEFINE_TYPELIST(IMcSession)
public:
    virtual ~IMcSession() = default;
    
    virtual void send(const QString &msg) noexcept = 0;
    virtual void send(const QJsonObject &jsonObj) noexcept = 0;
    virtual void send(QObject *obj) noexcept = 0;
    virtual void close() noexcept = 0;
};

MC_DECL_METATYPE(IMcSession)
