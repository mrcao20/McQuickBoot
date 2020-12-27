#pragma once

#include "../IMcControllerContainer.h"

#include <QMap>

QT_BEGIN_NAMESPACE
class QMetaMethod;
QT_END_NAMESPACE

MC_FORWARD_DECL_CLASS(IMcQuickBoot)
MC_FORWARD_DECL_STRUCT(McSequentialMetaId)
MC_FORWARD_DECL_STRUCT(McAssociativeMetaId)

MC_FORWARD_DECL_PRIVATE_DATA(McControllerContainer);

class MCQUICKBOOT_EXPORT McControllerContainer 
        : public QObject
        , public IMcControllerContainer 
{
    Q_OBJECT
    MC_DECL_INIT(McControllerContainer)
    MC_COMPONENT
    MC_BEANNAME("controllerContainer")
public:
    Q_INVOKABLE explicit McControllerContainer(QObject *parent = nullptr);
    ~McControllerContainer() override;

    void init(const IMcQuickBoot *boot) noexcept;

    QVariant invoke(const QString &uri, const QVariant &body) noexcept override;
    QVariant invoke(const QString &uri) noexcept override;
    QVariant invoke(const QString &uri, const QJsonObject &data) noexcept override;
    QVariant invoke(const QString &uri, const QVariantList &data) noexcept override;
    QVariant invoke(const QString &uri, const QVariantMap &data) noexcept override;

private:
    bool splitBeanAndFunc(const QString &uri,
                          QObjectPtr &bean,
                          QString &func,
                          QVariant &errRet) noexcept;

    QVariant invokeForUri(QObjectConstPtrRef bean,
                          const QString &func,
                          const QVariantMap &args) noexcept;
    QVariant invokeForUri(QObjectConstPtrRef bean,
                          const QString &func,
                          const QVariantList &args) noexcept;

    QVariantMap splitParam(const QString &param) noexcept;

    bool isMethodMatching(const QMetaMethod &m, const QList<QString> &argNames) noexcept;
    bool isMethodMatching(const QMetaMethod &m, const QVariantList &args) noexcept;

    void removeDuplication(QList<QString> &argNames,
                           QList<QByteArray> &paramNames,
                           QList<QByteArray> &paramTypes) noexcept;

    QVariant invokeForArgs(QObjectConstPtrRef bean,
                           const QMetaMethod &method,
                           const QVariantMap &args) noexcept;
    QVariant invokeForArgs(QObjectConstPtrRef bean,
                           const QMetaMethod &method,
                           const QVariantList &args) noexcept;
    QVariantList makeValues(const QMetaMethod &method,
                            const QVariantMap &args,
                            int maxParamSize,
                            QVariant *errMsg = nullptr,
                            bool *ok = nullptr) noexcept;
    QVariantList makeValues(const QMetaMethod &method,
                            const QVariantList &args,
                            int maxParamSize,
                            QVariant *errMsg = nullptr,
                            bool *ok = nullptr) noexcept;
    QVariant makeValue(const QByteArray &typeName, const QVariant &arg) noexcept;
    
    QVariant makePlanValue(const QByteArray &typeName, const QVariant &arg) noexcept;
    QVariant makeListValue(const QVariant &arg, McSequentialMetaIdConstPtrRef seqMetaId) noexcept;
    QVariant makeMapValue(const QVariant &arg, McAssociativeMetaIdConstPtrRef assMetaId) noexcept;
    QVariant makeObjectValue(const QByteArray &typeName, const QVariant &arg) noexcept;
    bool isSharedPointerObject(const QByteArray &typeName, QByteArray &objTypeName) noexcept;
    
    QVariant ok(const QVariant &val) const noexcept;
    QVariant fail(const QString &val) const noexcept;

private:
    MC_DECL_PRIVATE(McControllerContainer)
};

MC_DECL_METATYPE(McControllerContainer)
