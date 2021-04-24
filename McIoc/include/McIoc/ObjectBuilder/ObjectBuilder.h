#pragma once

#include <QObject>
#include <QSharedPointer>

#include "McIoc/McMacroGlobal.h"

namespace McPrivate {

class MCIOC_EXPORT IQObjectBuilder
{
public:
    virtual ~IQObjectBuilder() = default;

    virtual QObject *create() noexcept = 0;

    static void addQObjectBuilder(int id, const QSharedPointer<IQObjectBuilder> &builder) noexcept;
    static QSharedPointer<IQObjectBuilder> getQObjectBuilder(int id) noexcept;
};

template<typename T>
class QObjectBuilder : public IQObjectBuilder
{
public:
    QObject *create() noexcept override { return new T(); }
};

MC_DECL_POINTER(IQObjectBuilder)

} // namespace McPrivate
