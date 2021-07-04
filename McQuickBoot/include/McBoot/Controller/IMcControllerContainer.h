#pragma once

#include <QVariant>

#include "../McBootGlobal.h"

class IMcControllerContainer 
{
public:
    virtual ~IMcControllerContainer() = default;
    
    virtual QVariant invoke(const QString &uri, const QVariant &body) noexcept = 0;
    virtual QVariant invoke(const QString &uri) noexcept = 0;
    virtual QVariant invoke(const QString &uri, const QJsonObject &data) noexcept = 0;
    virtual QVariant invoke(const QString &uri, const QVariantList &data) noexcept = 0;
    virtual QVariant invoke(const QString &uri, const QVariantMap &data) noexcept = 0;
};

MC_DECL_METATYPE(IMcControllerContainer)
