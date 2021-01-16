#include "McOrm/SqlSlot/impl/McAbstractSqlSlot.h"

#include <QMetaMethod>
#include <QDebug>

#include <McIoc/McGlobal.h>
#include <McIoc/BeanFactory/impl/McMetaTypeId.h>

#include "McOrm/McOrmMacroGlobal.h"

MC_DECL_PRIVATE_DATA(McAbstractSqlSlot)
QMetaMethod sig;
QByteArray additionalInfo;
MC_DECL_PRIVATE_DATA_END

McAbstractSqlSlot::McAbstractSqlSlot(QObject *parent) noexcept
    : QObject(parent)
{
    MC_NEW_PRIVATE_DATA(McAbstractSqlSlot);
}

McAbstractSqlSlot::~McAbstractSqlSlot()
{
}

void McAbstractSqlSlot::init(QObject *bean, const QMetaMethod &sig, const QByteArray &additionalInfo) noexcept
{
    setParent(bean);
    
    QList<int> types;
    types.append(sig.returnType());
    for(int i = 0; i < sig.parameterCount(); ++i) {
        types.append(sig.parameterType(i));
    }
    if(!checkType(types) && additionalInfo.isEmpty()) {
        qCritical("the method: '%s' return type or parameter type invalid", sig.methodSignature().data());
        return;
    }
    d->sig = sig;
    d->additionalInfo = additionalInfo;
    
    QMetaObject::connect(bean, sig.methodIndex(), this, metaObject()->methodCount()
                         , Qt::DirectConnection | Qt::UniqueConnection);
}

int McAbstractSqlSlot::qt_metacall(QMetaObject::Call c, int id, void **arguments)
{
    id = QObject::qt_metacall(c, id, arguments);
    if (id < 0 || c != QMetaObject::InvokeMetaMethod)
        return id;
    query(arguments, d->sig, d->additionalInfo);
    return -1;
}

const QMetaObject *McAbstractSqlSlot::getMetaObject(int type) noexcept
{
    auto sharedMetaIds = McMetaTypeId::sharedPointerIds();
    auto seqMetaIds = McMetaTypeId::sequentialIds();
    
    const QMetaObject *mo = nullptr;
    if(sharedMetaIds.contains(type)) {
        mo = QMetaType::metaObjectForType(sharedMetaIds.value(type)->qobjectPointerId);
    } else if (seqMetaIds.contains(type)) {
        auto seq = seqMetaIds.value(type);
        if (sharedMetaIds.contains(seq->valueId)) {
            mo = QMetaType::metaObjectForType(sharedMetaIds.value(seq->valueId)->qobjectPointerId);
        } else {
            mo = QMetaType::metaObjectForType(seq->valueId);
        }
    }
    return mo;
}

bool McAbstractSqlSlot::isPo(int type) noexcept
{
    if(type < QMetaType::User) {
        return false;
    }
    auto mo = getMetaObject(type);
    if(mo == nullptr) {
        return false;
    }
    if(mo->indexOfClassInfo(MC_DB_TABLE_TAG) != -1) {
        return true;
    }
    return false;
}

bool McAbstractSqlSlot::checkType(const QList<int> &types) noexcept
{
    if(types.isEmpty()) {
        return false;
    }
    int paramPoNum = 0;
    for(int i = 1; i < types.length(); ++i) {
        auto type = types.at(i);
        if(isPo(type)) {
            ++paramPoNum;
        }
    }
    bool isReturnPo = isPo(types.first());
    if(isReturnPo && paramPoNum == 0) {
        return true;
    }
    if(!isReturnPo && paramPoNum == 1 && types.length() == 2) {
        return true;
    }
    return false;
}
