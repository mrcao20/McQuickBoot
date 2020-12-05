#pragma once

#include "McAbstractSqlSlot.h"

class McSelectSqlSlot : public McAbstractSqlSlot
{
public:
    McSelectSqlSlot(QObject *parent = nullptr) noexcept;
    
protected:
    void query(void **args, const QMetaMethod &sig, const QByteArray &additionalInfo) noexcept override;
    
private:
    QVariant queryForReturn(void **args, const QMetaMethod &sig) noexcept;
    void queryForParam(QObjectConstPtrRef po) noexcept;
    void queryForParam(const QVariantList &poVars) noexcept;
    
    QMap<QString, QString> getProColMap(const QMetaObject *mo) noexcept;
};
