#pragma once

#include <QObject>
#include <QSharedPointer>

#include "McIoc/McMacroGlobal.h"

namespace McPrivate {

MC_FORWARD_DECL_CLASS(IQObjectBuilder);

class MCIOC_EXPORT IQObjectBuilder
{
public:
    virtual ~IQObjectBuilder() = default;

    virtual QObject *create() noexcept = 0;

    static void addQObjectBuilder(int id, IQObjectBuilderConstPtrRef builder) noexcept;
    template<typename T>
    static void addQObjectBuilder() noexcept;
    static IQObjectBuilderPtr getQObjectBuilder(int id) noexcept;
};

template<typename T>
class QObjectBuilder : public IQObjectBuilder
{
public:
    QObject *create() noexcept override { return new T(); }
};

template<typename T>
using QObjectBuilderPtr = QSharedPointer<QObjectBuilder<T>>;

template<typename T>
void IQObjectBuilder::addQObjectBuilder() noexcept
{
    auto id = qMetaTypeId<std::remove_pointer_t<T> *>();
    auto builder = QObjectBuilderPtr<std::remove_pointer_t<T>>::create();
    IQObjectBuilder::addQObjectBuilder(id, builder);
}

} // namespace McPrivate
