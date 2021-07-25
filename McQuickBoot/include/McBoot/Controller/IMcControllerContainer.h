#pragma once

#include <QVariant>

#include "../Requestor/McRequest.h"

class IMcControllerContainer 
{
public:
    virtual ~IMcControllerContainer() = default;

    virtual QVariant invoke(const QString &uri,
                            const QVariant &body,
                            const McRequest &request) noexcept = 0;
    virtual QVariant invoke(const QString &uri, const McRequest &request) noexcept = 0;
    virtual QVariant invoke(const QString &uri,
                            const QJsonObject &data,
                            const McRequest &request) noexcept = 0;
    virtual QVariant invoke(const QString &uri,
                            const QVariantList &data,
                            const McRequest &request) noexcept = 0;
    virtual QVariant invoke(const QString &uri,
                            const QVariantMap &data,
                            const McRequest &request) noexcept = 0;
};

MC_DECL_METATYPE(IMcControllerContainer)
