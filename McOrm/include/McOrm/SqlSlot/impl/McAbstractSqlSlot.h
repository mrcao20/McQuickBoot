#pragma once

#include <QObject>
#include "../IMcSqlSlot.h"

#include <McIoc/McGlobal.h>

MC_FORWARD_DECL_PRIVATE_DATA(McAbstractSqlSlot);

class McAbstractSqlSlot : public QObject, public IMcSqlSlot
{
public:
    McAbstractSqlSlot(QObject *parent = nullptr) noexcept;
    ~McAbstractSqlSlot() override;
    
    void init(QObject *bean, const QMetaMethod &sig, const QByteArray &additionalInfo) noexcept override;
    
    int qt_metacall(QMetaObject::Call c, int id, void **arguments) override;
    
protected:
    virtual void query(void **args, const QMetaMethod &sig, const QByteArray &additionalInfo) noexcept = 0;
    
protected:
    const QMetaObject *getMetaObject(int type) noexcept;
    bool isPo(int type) noexcept;
    
private:
    bool checkType(const QList<int> &types) noexcept;
    
private:
    MC_DECL_PRIVATE(McAbstractSqlSlot)
};
