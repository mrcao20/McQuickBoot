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
    using McAbstractRequestor::getBean;
    using McAbstractRequestor::getModel;
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
    template<typename Func>
    QMetaObject::Connection connect(const QString &sender,
                                    const QString &signal,
                                    const typename QtPrivate::FunctionPointer<Func>::Object *receiver,
                                    Func callback,
                                    Qt::ConnectionType type = Qt::AutoConnection) noexcept
    {
        typedef QtPrivate::FunctionPointer<Func> FuncType;

        return connectImpl(sender,
                           signal,
                           receiver,
                           new QtPrivate::QSlotObject<Func,
                                                      typename FuncType::Arguments,
                                                      typename FuncType::ReturnType>(callback),
                           type);
    }
    template<typename Func>
    typename std::enable_if<int(QtPrivate::FunctionPointer<Func>::ArgumentCount) >= 0
                                && !QtPrivate::FunctionPointer<Func>::IsPointerToMemberFunction,
                            QMetaObject::Connection>::type
    connect(const QString &sender,
            const QString &signal,
            const QObject *receiver,
            Func callback,
            Qt::ConnectionType type = Qt::AutoConnection) noexcept
    {
        typedef QtPrivate::FunctionPointer<Func> FuncType;

        return connectImpl(sender,
                           signal,
                           receiver,
                           new QtPrivate::QStaticSlotObject<Func,
                                                            typename FuncType::Arguments,
                                                            typename FuncType::ReturnType>(callback),
                           type);
    }
    template<typename Func>
    typename std::enable_if<QtPrivate::FunctionPointer<Func>::ArgumentCount == -1,
                            QMetaObject::Connection>::type
    connect(const QString &sender,
            const QString &signal,
            const QObject *receiver,
            Func callback,
            Qt::ConnectionType type = Qt::AutoConnection) noexcept
    {
        typedef McPrivate::LambdaType<Func> FuncType;

        return connectImpl(sender,
                           signal,
                           receiver,
                           new QtPrivate::QFunctorSlotObject<Func,
                                                             int(FuncType::ArgumentCount),
                                                             typename FuncType::Arguments,
                                                             typename FuncType::ReturnType>(
                               std::move(callback)),
                           type);
    }
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

    template<typename T>
    T getBean(const QString &name) const noexcept
    {
        return getBeanToVariant(name).value<T>();
    }
    template<typename T>
    T getModel(const QString &name) const noexcept
    {
        QObject *obj = getModel(name);
        return qobject_cast<T>(obj);
    }

private:
    QMetaObject::Connection connectImpl(const QString &sender,
                                        const QString &signal,
                                        const QObject *receiver,
                                        QtPrivate::QSlotObjectBase *slot,
                                        Qt::ConnectionType type) noexcept;
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
