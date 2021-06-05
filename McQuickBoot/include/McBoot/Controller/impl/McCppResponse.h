#pragma once

#include "McAbstractResponse.h"

MC_FORWARD_DECL_PRIVATE_DATA(McCppResponse);

class MCQUICKBOOT_EXPORT McCppResponse : public McAbstractResponse
{
    Q_OBJECT
    MC_DECL_INIT(McCppResponse)
    template<typename T>
    struct MetaTypeIdHelper
    {
        static int metaTypeId() { return qMetaTypeId<typename T::Car>(); }
    };
    template<>
    struct MetaTypeIdHelper<QtPrivate::List<>>
    {
        static int metaTypeId() { return -1; }
    };

public:
    explicit McCppResponse(QObject *parent = nullptr);
    ~McCppResponse() override;

    QPointer<McCppResponse> capture();

    template<typename Func>
    McCppResponse &then(const typename QtPrivate::FunctionPointer<Func>::Object *recever,
                        Func callback) noexcept
    {
        typedef QtPrivate::FunctionPointer<Func> FuncType;

        Q_STATIC_ASSERT_X(
            int(FuncType::ArgumentCount) <= 1,
            "The number of parameters of callback function can only be less than or equal to 1");

        return thenImpl(McPrivate::QVariantSelector<typename FuncType::Arguments>::Value,
                        MetaTypeIdHelper<typename FuncType::Arguments>::metaTypeId(),
                        recever,
                        new QtPrivate::QSlotObject<Func,
                                                   typename FuncType::Arguments,
                                                   typename FuncType::ReturnType>(callback));
    }
    template<typename Func>
    typename std::enable_if<int(QtPrivate::FunctionPointer<Func>::ArgumentCount) >= 0
                                && !QtPrivate::FunctionPointer<Func>::IsPointerToMemberFunction,
                            McCppResponse &>::type
    then(Func callback) noexcept
    {
        typedef QtPrivate::FunctionPointer<Func> FuncType;

        Q_STATIC_ASSERT_X(
            int(FuncType::ArgumentCount) <= 1,
            "The number of parameters of callback function can only be less than or equal to 1");

        return thenImpl(McPrivate::QVariantSelector<typename FuncType::Arguments>::Value,
                        MetaTypeIdHelper<typename FuncType::Arguments>::metaTypeId(),
                        nullptr,
                        new QtPrivate::QStaticSlotObject<Func,
                                                         typename FuncType::Arguments,
                                                         typename FuncType::ReturnType>(callback));
    }
    template<typename Func>
    typename std::enable_if<QtPrivate::FunctionPointer<Func>::ArgumentCount == -1,
                            McCppResponse &>::type
    then(Func callback) noexcept
    {
        typedef McPrivate::LambdaType<Func> FuncType;

        Q_STATIC_ASSERT_X(
            int(FuncType::ArgumentCount) <= 1,
            "The number of parameters of callback function can only be less than or equal to 1");

        return thenImpl(McPrivate::QVariantSelector<typename FuncType::Arguments>::Value,
                        MetaTypeIdHelper<typename FuncType::Arguments>::metaTypeId(),
                        nullptr,
                        new QtPrivate::QFunctorSlotObject<Func,
                                                          int(FuncType::ArgumentCount),
                                                          typename FuncType::Arguments,
                                                          typename FuncType::ReturnType>(
                            std::move(callback)));
    }

    template<typename Func>
    McCppResponse &syncThen(const typename QtPrivate::FunctionPointer<Func>::Object *recever,
                            Func callback) noexcept
    {
        typedef QtPrivate::FunctionPointer<Func> FuncType;

        Q_STATIC_ASSERT_X(
            int(FuncType::ArgumentCount) <= 1,
            "The number of parameters of callback function can only be less than or equal to 1");

        return syncThenImpl(McPrivate::QVariantSelector<typename FuncType::Arguments>::Value,
                            MetaTypeIdHelper<typename FuncType::Arguments>::metaTypeId(),
                            recever,
                            new QtPrivate::QSlotObject<Func,
                                                       typename FuncType::Arguments,
                                                       typename FuncType::ReturnType>(callback));
    }
    template<typename Func>
    typename std::enable_if<int(QtPrivate::FunctionPointer<Func>::ArgumentCount) >= 0
                                && !QtPrivate::FunctionPointer<Func>::IsPointerToMemberFunction,
                            McCppResponse &>::type
    syncThen(Func callback) noexcept
    {
        typedef QtPrivate::FunctionPointer<Func> FuncType;

        Q_STATIC_ASSERT_X(
            int(FuncType::ArgumentCount) <= 1,
            "The number of parameters of callback function can only be less than or equal to 1");

        return syncThenImpl(McPrivate::QVariantSelector<typename FuncType::Arguments>::Value,
                            MetaTypeIdHelper<typename FuncType::Arguments>::metaTypeId(),
                            nullptr,
                            new QtPrivate::QStaticSlotObject<Func,
                                                             typename FuncType::Arguments,
                                                             typename FuncType::ReturnType>(
                                callback));
    }
    template<typename Func>
    typename std::enable_if<QtPrivate::FunctionPointer<Func>::ArgumentCount == -1,
                            McCppResponse &>::type
    syncThen(Func callback) noexcept
    {
        typedef McPrivate::LambdaType<Func> FuncType;

        Q_STATIC_ASSERT_X(
            int(FuncType::ArgumentCount) <= 1,
            "The number of parameters of callback function can only be less than or equal to 1");

        return syncThenImpl(McPrivate::QVariantSelector<typename FuncType::Arguments>::Value,
                            MetaTypeIdHelper<typename FuncType::Arguments>::metaTypeId(),
                            nullptr,
                            new QtPrivate::QFunctorSlotObject<Func,
                                                              int(FuncType::ArgumentCount),
                                                              typename FuncType::Arguments,
                                                              typename FuncType::ReturnType>(
                                std::move(callback)));
    }

    template<typename Func>
    McCppResponse &asyncThen(const typename QtPrivate::FunctionPointer<Func>::Object *recever,
                             Func callback) noexcept
    {
        typedef QtPrivate::FunctionPointer<Func> FuncType;

        Q_STATIC_ASSERT_X(
            int(FuncType::ArgumentCount) <= 1,
            "The number of parameters of callback function can only be less than or equal to 1");

        return asyncThenImpl(McPrivate::QVariantSelector<typename FuncType::Arguments>::Value,
                             MetaTypeIdHelper<typename FuncType::Arguments>::metaTypeId(),
                             recever,
                             new QtPrivate::QSlotObject<Func,
                                                        typename FuncType::Arguments,
                                                        typename FuncType::ReturnType>(callback));
    }
    template<typename Func>
    typename std::enable_if<int(QtPrivate::FunctionPointer<Func>::ArgumentCount) >= 0
                                && !QtPrivate::FunctionPointer<Func>::IsPointerToMemberFunction,
                            McCppResponse &>::type
    asyncThen(Func callback) noexcept
    {
        typedef QtPrivate::FunctionPointer<Func> FuncType;

        Q_STATIC_ASSERT_X(
            int(FuncType::ArgumentCount) <= 1,
            "The number of parameters of callback function can only be less than or equal to 1");

        return asyncThenImpl(McPrivate::QVariantSelector<typename FuncType::Arguments>::Value,
                             MetaTypeIdHelper<typename FuncType::Arguments>::metaTypeId(),
                             nullptr,
                             new QtPrivate::QStaticSlotObject<Func,
                                                              typename FuncType::Arguments,
                                                              typename FuncType::ReturnType>(
                                 callback));
    }
    template<typename Func>
    typename std::enable_if<QtPrivate::FunctionPointer<Func>::ArgumentCount == -1,
                            McCppResponse &>::type
    asyncThen(Func callback) noexcept
    {
        typedef McPrivate::LambdaType<Func> FuncType;

        Q_STATIC_ASSERT_X(
            int(FuncType::ArgumentCount) <= 1,
            "The number of parameters of callback function can only be less than or equal to 1");

        return asyncThenImpl(McPrivate::QVariantSelector<typename FuncType::Arguments>::Value,
                             MetaTypeIdHelper<typename FuncType::Arguments>::metaTypeId(),
                             nullptr,
                             new QtPrivate::QFunctorSlotObject<Func,
                                                               int(FuncType::ArgumentCount),
                                                               typename FuncType::Arguments,
                                                               typename FuncType::ReturnType>(
                                 std::move(callback)));
    }

    template<typename Func>
    McCppResponse &error(const typename QtPrivate::FunctionPointer<Func>::Object *recever,
                         Func func) noexcept
    {
        typedef QtPrivate::FunctionPointer<Func> FuncType;

        Q_STATIC_ASSERT_X(
            int(FuncType::ArgumentCount) <= 1,
            "The number of parameters of callback function can only be less than or equal to 1");

        return errorImpl(McPrivate::QVariantSelector<typename FuncType::Arguments>::Value,
                         MetaTypeIdHelper<typename FuncType::Arguments>::metaTypeId(),
                         recever,
                         new QtPrivate::QSlotObject<Func,
                                                    typename FuncType::Arguments,
                                                    typename FuncType::ReturnType>(func));
    }
    template<typename Func>
    typename std::enable_if<int(QtPrivate::FunctionPointer<Func>::ArgumentCount) >= 0
                                && !QtPrivate::FunctionPointer<Func>::IsPointerToMemberFunction,
                            McCppResponse &>::type
    error(Func func) noexcept
    {
        typedef QtPrivate::FunctionPointer<Func> FuncType;

        Q_STATIC_ASSERT_X(
            int(FuncType::ArgumentCount) <= 1,
            "The number of parameters of callback function can only be less than or equal to 1");

        return errorImpl(McPrivate::QVariantSelector<typename FuncType::Arguments>::Value,
                         MetaTypeIdHelper<typename FuncType::Arguments>::metaTypeId(),
                         nullptr,
                         new QtPrivate::QStaticSlotObject<Func,
                                                          typename FuncType::Arguments,
                                                          typename FuncType::ReturnType>(func));
    }
    template<typename Func>
    typename std::enable_if<QtPrivate::FunctionPointer<Func>::ArgumentCount == -1,
                            McCppResponse &>::type
    error(Func func) noexcept
    {
        typedef McPrivate::LambdaType<Func> FuncType;

        Q_STATIC_ASSERT_X(
            int(FuncType::ArgumentCount) <= 1,
            "The number of parameters of callback function can only be less than or equal to 1");

        return errorImpl(McPrivate::QVariantSelector<typename FuncType::Arguments>::Value,
                         MetaTypeIdHelper<typename FuncType::Arguments>::metaTypeId(),
                         nullptr,
                         new QtPrivate::QFunctorSlotObject<Func,
                                                           int(FuncType::ArgumentCount),
                                                           typename FuncType::Arguments,
                                                           typename FuncType::ReturnType>(
                             std::move(func)));
    }

protected:
    void callCallback() noexcept override;
    void callError() noexcept override;

private:
    McCppResponse &thenImpl(bool isQVariant,
                            int argumentId,
                            const QObject *recever,
                            QtPrivate::QSlotObjectBase *callback) noexcept;
    McCppResponse &syncThenImpl(bool isQVariant,
                                int argumentId,
                                const QObject *recever,
                                QtPrivate::QSlotObjectBase *callback) noexcept;
    McCppResponse &asyncThenImpl(bool isQVariant,
                                 int argumentId,
                                 const QObject *recever,
                                 QtPrivate::QSlotObjectBase *callback) noexcept;
    McCppResponse &errorImpl(bool isQVariant,
                             int argumentId,
                             const QObject *recever,
                             QtPrivate::QSlotObjectBase *func) noexcept;
    void call(QtPrivate::QSlotObjectBase *func) noexcept;

private:
    MC_DECL_PRIVATE(McCppResponse)
};
