#pragma once

#include "../McBootGlobal.h"

QT_BEGIN_NAMESPACE
class QJSValue;
QT_END_NAMESPACE

class McQmlSocket;

class IMcQmlSocketContainer 
{
public:
    virtual ~IMcQmlSocketContainer() = default;
    
    virtual McQmlSocket *addConnect(const QString &uri) noexcept = 0;
    virtual McQmlSocket *addConnect(const QString &uri, const QJSValue &data) noexcept = 0;
};

MC_DECL_POINTER(IMcQmlSocketContainer)
