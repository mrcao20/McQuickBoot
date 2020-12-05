#pragma once

class QObject;
class QMetaMethod;
class QByteArray;

class IMcSqlSlot
{
public:
    virtual ~IMcSqlSlot() = default;
    
    virtual void init(QObject *bean, const QMetaMethod &sig, const QByteArray &additionalInfo) noexcept = 0;
};
