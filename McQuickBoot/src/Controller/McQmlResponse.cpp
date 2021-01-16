#include "McBoot/Controller/impl/McQmlResponse.h"

#include <QJSEngine>
#include <QJSValue>
#include <QJsonObject>
#include <QMetaClassInfo>
#include <QVariant>

#include <McIoc/BeanFactory/impl/McMetaTypeId.h>
#include <McIoc/Utils/McScopedFunction.h>

#include "McBoot/Utils/McJsonUtils.h"

MC_DECL_PRIVATE_DATA(McQmlResponse)
QJSValue callback;
MC_DECL_PRIVATE_DATA_END

MC_INIT(McQmlResponse)
qRegisterMetaType<McQmlResponse *>("McQmlResponse");
MC_INIT_END

McQmlResponse::McQmlResponse(QObject *parent) : McAbstractResponse(parent)
{
    MC_NEW_PRIVATE_DATA(McQmlResponse);
}

McQmlResponse::~McQmlResponse()
{
}

McQmlResponse *McQmlResponse::then(const QJSValue &callback) noexcept 
{
    d->callback = callback;
    setAsyncCall(false);
    return this;
}

McQmlResponse *McQmlResponse::syncThen(const QJSValue &callback) noexcept
{
    d->callback = callback;
    setAsyncCall(false);
    return this;
}

McQmlResponse *McQmlResponse::asyncThen(const QJSValue &callback) noexcept
{
    d->callback = callback;
    setAsyncCall(true);
    return this;
}

void McQmlResponse::callCallback() noexcept 
{
    McScopedFunction func([this](){
        this->deleteLater();
    });
    Q_UNUSED(func)
    
    if(!d->callback.isCallable()) {
        return;
    }
#if QT_VERSION < QT_VERSION_CHECK(5, 5, 0)
    auto engine = QQmlEngine::contextForObject(this)->engine();
#else
    auto engine = qjsEngine(this);
#endif
    if(!engine) {
        return;
    }
    auto body = serialize(this->body());
    auto arg = engine->toScriptValue(body);
    d->callback.call(QJSValueList() << arg);
}

QVariant McQmlResponse::serialize(const QVariant &origin) noexcept
{
    auto originId = origin.userType();
    auto qobjectIds = McMetaTypeId::qobjectPointerIds();
    auto sharedIds = McMetaTypeId::sharedPointerIds();
    auto gadgetIds = McMetaTypeId::gadgetIds();
    for (auto gadgetId : gadgetIds) {
        if (gadgetId->pointerId == originId || gadgetId->sharedId == originId) {
            return serialize(*reinterpret_cast<const void *const *>(origin.constData()),
                             QMetaType::metaObjectForType(gadgetId->gadgetId),
                             origin);
        }
    }
    if (qobjectIds.contains(originId)) {
        return serialize(origin.value<QObject *>(), origin);
    } else if (sharedIds.contains(originId)) {
        return serialize(origin.value<QObjectPtr>().data(), origin);
    } else {
        return origin;
    }
}

QVariant McQmlResponse::serialize(QObject *obj, const QVariant &origin) noexcept
{
    auto metaObj = obj->metaObject();
    if (!isSerialize(metaObj)) {
        return origin;
    }
    return McJsonUtils::toJson(obj);
}

QVariant McQmlResponse::serialize(const void *obj,
                                  const QMetaObject *metaObj,
                                  const QVariant &origin) noexcept
{
    if (!isSerialize(metaObj)) {
        return origin;
    }
    return McJsonUtils::toJson(obj, metaObj);
}

bool McQmlResponse::isSerialize(const QMetaObject *metaObj) noexcept
{
    auto serializeIndex = metaObj->indexOfClassInfo(MC_SERIALIZATION_TAG);
    if (serializeIndex == -1) {
        return false;
    }
    auto classInfo = metaObj->classInfo(serializeIndex);
    if (qstrcmp(classInfo.value(), MC_JSON_SERIALIZATION_TAG) != 0) {
        return false;
    }
    return true;
}
