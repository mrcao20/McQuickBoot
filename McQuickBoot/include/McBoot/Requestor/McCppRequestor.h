#pragma once

#include "McAbstractRequestor.h"

MC_FORWARD_DECL_PRIVATE_DATA(McCppRequestor);

class McCppResponse;

class MCQUICKBOOT_EXPORT McCppRequestor : public McAbstractRequestor
{
    Q_OBJECT
    MC_DECL_INIT(McCppRequestor)
    MC_TYPELIST(McAbstractRequestor)
    MC_COMPONENT
    MC_BEANNAME("cppRequestor")
public:
    using QObject::connect;
    using QObject::disconnect;

    Q_INVOKABLE explicit McCppRequestor(QObject *parent = nullptr);
    ~McCppRequestor() override;

    QMetaObject::Connection connect(const QString &sender,
                                    const QString &signal,
                                    const QString &receiver,
                                    const QString &slot,
                                    Qt::ConnectionType type = Qt::AutoConnection) noexcept;
    QMetaObject::Connection connect(const QString &sender,
                                    const QString &signal,
                                    QObject *receiver,
                                    const QString &slot,
                                    Qt::ConnectionType type = Qt::AutoConnection) noexcept;
    bool disconnect(const QString &sender,
                    const QString &signal,
                    const QString &receiver,
                    const QString &slot) noexcept;

    bool disconnect(const QString &sender,
                    const QString &signal,
                    QObject *receiver,
                    const QString &slot) noexcept;

    McCppResponse &invoke(const QString &uri) noexcept;
    McCppResponse &invoke(const QString &uri, const QJsonObject &data) noexcept;
    McCppResponse &invoke(const QString &uri, const QVariant &data) noexcept;
    McCppResponse &invoke(const QString &uri, const QVariantList &data) noexcept;
    template<typename... Args>
    McCppResponse &invoke(const QString &uri, const Args &... args) noexcept
    {
        QVariantList vars;
        (vars << ... << toQVariant(args));
        return invoke(uri, vars);
    }

    QVariant syncInvoke(const QString &uri) noexcept;
    QVariant syncInvoke(const QString &uri, const QJsonObject &data) noexcept;
    QVariant syncInvoke(const QString &uri, const QVariant &data) noexcept;
    QVariant syncInvoke(const QString &uri, const QVariantList &data) noexcept;
    template<typename... Args>
    QVariant syncInvoke(const QString &uri, const Args &... args) noexcept
    {
        QVariantList vars;
        (vars << ... << toQVariant(args));
        return syncInvoke(uri, vars);
    }

private:
    template<typename T>
    typename std::enable_if<McPrivate::IsQVariantHelper<T>::Value, QVariant>::type toQVariant(
        const T &t)
    {
        return QVariant::fromValue(t);
    }
    QVariant toQVariant(const QVariant &t) { return t; }
    template<typename T>
    typename std::enable_if<McPrivate::IsQVariantHelper<T>::Value2, QVariant>::type toQVariant(
        const T &t)
    {
        return QVariant(t);
    }

private:
    MC_DECL_PRIVATE(McCppRequestor)
};

MC_DECL_METATYPE(McCppRequestor)
