#include "McIoc/BeanFactory/impl/McMetaTypeId.h"

#include <QGlobalStatic>

Q_GLOBAL_STATIC(QVector<int>, mcQObjectPointerIds)
Q_GLOBAL_STATIC(QVector<int>, mcSharedPointerIds)
Q_GLOBAL_STATIC(McSequentialType, mcSequentialIds)
Q_GLOBAL_STATIC(McAssociativeType, mcAssociativeIds)

QVector<int> McMetaTypeId::qobjectPointerIds() noexcept
{
    return *mcQObjectPointerIds;
}

void McMetaTypeId::addQObjectPointerIds(int id) noexcept
{
    auto ids = mcQObjectPointerIds;
    if(ids->contains(id)) {
        return;
    }
    ids->append(id);
}

QVector<int> McMetaTypeId::sharedPointerIds() noexcept
{
    return *mcSharedPointerIds;
}

void McMetaTypeId::addSharedPointerId(int id) noexcept
{
    auto ids = mcSharedPointerIds();
    if(ids->contains(id)) {
        return;
    }
    ids->append(id);
}

McSequentialType McMetaTypeId::sequentialIds() noexcept
{
    return *mcSequentialIds;
}

void McMetaTypeId::addSequentialId(int id, int valueId) noexcept
{
    auto ids = mcSequentialIds();
    if(ids->contains(id)) {
        return;
    }
    auto seqId = McSequentialMetaIdPtr::create();
    seqId->id = id;
    seqId->valueId = valueId;
    ids->insert(id, seqId);
}

McAssociativeType McMetaTypeId::associativeIds() noexcept
{
    return *mcAssociativeIds;
}

void McMetaTypeId::addAssociativeId(int id, int keyId, int valueId) noexcept
{
    auto ids = mcAssociativeIds();
    if(ids->contains(id)) {
        return;
    }
    auto assId = McAssociativeMetaIdPtr::create();
    assId->id = id;
    assId->keyId = keyId;
    assId->valueId = valueId;
    ids->insert(id, assId);
}
