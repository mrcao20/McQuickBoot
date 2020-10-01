#include "McIoc/BeanFactory/impl/McMetaTypeId.h"

#include <QGlobalStatic>

Q_GLOBAL_STATIC(McPointerType, mcQObjectPointerIds)
Q_GLOBAL_STATIC(McPointerType, mcSharedPointerIds)
Q_GLOBAL_STATIC(McSequentialType, mcSequentialIds)
Q_GLOBAL_STATIC(McAssociativeType, mcAssociativeIds)

McPointerType McMetaTypeId::qobjectPointerIds() noexcept
{
    return *mcQObjectPointerIds;
}

void McMetaTypeId::addQObjectPointerIds(int id, int sharedId) noexcept
{
    auto ids = mcQObjectPointerIds();
    if(ids->contains(id)) {
        return;
    }
    McPointerMetaIdPtr pId = McPointerMetaIdPtr::create();
    pId->qobjectPointerId = id;
    pId->sharedPointerId = sharedId;
    ids->insert(id, pId);
}

McPointerType McMetaTypeId::sharedPointerIds() noexcept
{
    return *mcSharedPointerIds;
}

void McMetaTypeId::addSharedPointerId(int id, int qobjectId) noexcept
{
    auto ids = mcSharedPointerIds();
    if(ids->contains(id)) {
        return;
    }
    McPointerMetaIdPtr pId = McPointerMetaIdPtr::create();
    pId->sharedPointerId = id;
    pId->qobjectPointerId = qobjectId;
    ids->insert(id, pId);
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
