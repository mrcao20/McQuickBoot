#include "McIoc/BeanFactory/impl/McMetaTypeId.h"

#include <QGlobalStatic>

MC_GLOBAL_STATIC(McPointerType, mcQObjectPointerIds)
MC_GLOBAL_STATIC(McPointerType, mcSharedPointerIds)
MC_GLOBAL_STATIC(McSequentialType, mcSequentialIds)
MC_GLOBAL_STATIC(McAssociativeType, mcAssociativeIds)

McPointerType McMetaTypeId::qobjectPointerIds() noexcept
{
    return *mcQObjectPointerIds;
}

void McMetaTypeId::addQObjectPointerIds(int id, int sharedId) noexcept
{
    McPointerType *ids = mcQObjectPointerIds;
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
    McPointerType *ids = mcSharedPointerIds;
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
    McSequentialType *ids = mcSequentialIds;
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
    McAssociativeType *ids = mcAssociativeIds;
    if(ids->contains(id)) {
        return;
    }
    auto assId = McAssociativeMetaIdPtr::create();
    assId->id = id;
    assId->keyId = keyId;
    assId->valueId = valueId;
    ids->insert(id, assId);
}
