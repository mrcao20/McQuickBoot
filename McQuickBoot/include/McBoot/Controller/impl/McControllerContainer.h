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
    MC_INTERFACES(IMcControllerContainer)
    MC_COMPONENT("controllerContainer")
public:
    explicit McControllerContainer(QObject *parent = nullptr);
    ~McControllerContainer() override;

    void init(const IMcQuickBoot *boot) noexcept;

    QVariant invoke(const QString &uri,
                    const QVariant &body,
                    const McRequest &request) noexcept override;
    QVariant invoke(const QString &uri, const McRequest &request) noexcept override;
    QVariant invoke(const QString &uri,
                    const QJsonObject &data,
                    const McRequest &request) noexcept override;
    QVariant invoke(const QString &uri,
                    const QVariantList &data,
                    const McRequest &request) noexcept override;
    QVariant invoke(const QString &uri,
                    const QVariantMap &data,
                    const McRequest &request) noexcept override;

private:
    bool splitBeanAndFunc(const QString &uri,
                          QObjectPtr &bean,
                          QString &func,
                          QVariant &errRet) noexcept;

    QVariant invokeForUri(QObjectConstPtrRef bean,
                          const QString &func,
                          const QVariantMap &args,
                          const McRequest &request) noexcept;
    bool makeCallback(QVariantMap &args, const QMetaMethod &m) noexcept;
    QVariant invokeForUri(QObjectConstPtrRef bean,
                          const QString &func,
                          const QVariantList &args,
                          const McRequest &request) noexcept;

    QVariantMap splitParam(const QString &param) noexcept;

    bool isMethodMatching(const QMetaMethod &m, const QList<QString> &argNames) noexcept;
    bool isMethodMatching(const QMetaMethod &m, const QVariantList &args) noexcept;

    void removeDuplication(QList<QString> &argNames, QList<QByteArray> &paramNames) noexcept;

    QVariant invokeForArgs(QObjectConstPtrRef bean,
                           const QMetaMethod &method,
                           const QVariantMap &args,
                           const McRequest &request) noexcept;
    QVariant invokeForArgs(QObjectConstPtrRef bean,
                           const QMetaMethod &method,
                           const QVariantList &args,
                           const McRequest &request) noexcept;
    QVariantList makeValues(const QMetaMethod &method,
                            const QVariantMap &args,
                            int maxParamSize,
                            const McRequest &request,
                            QVariant *errMsg = nullptr,
                            bool *ok = nullptr) noexcept;
    QVariantList makeValues(const QMetaMethod &method,
                            const QVariantList &args,
                            int maxParamSize,
                            const McRequest &request,
                            QVariant *errMsg = nullptr,
                            bool *ok = nullptr) noexcept;

    QVariant ok(const QVariant &val) const noexcept;
    QVariant fail(const QString &val) const noexcept;

private:
    MC_DECL_PRIVATE(McControllerContainer)
};

MC_DECL_METATYPE(McControllerContainer)
