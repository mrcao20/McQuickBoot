#pragma once

#include "McAbstractAsyncCallback.h"

MC_FORWARD_DECL_PRIVATE_DATA(McCppAsyncCallback);

class MCQUICKBOOT_EXPORT McCppAsyncCallback : public McAbstractAsyncCallback
{
    Q_OBJECT
    MC_TYPELIST(McAbstractAsyncCallback)
    MC_DECL_POINTER(McCppAsyncCallback)

    template<typename T>
    struct QVariantSelector;
    template<typename... Args>
    struct QVariantSelector<QtPrivate::List<Args...>>
    {
        static QList<bool> isQVariants()
        {
            QList<bool> l;
            (l << ... << McPrivate::QVariantSelector<Args>::Value);
            return l;
        }
    };

public:
    McCppAsyncCallback() noexcept;
    ~McCppAsyncCallback() override;

    McCppAsyncCallback(const McCppAsyncCallback &o) noexcept;
    McCppAsyncCallback(McCppAsyncCallback &&o) noexcept;
    void operator=(const McCppAsyncCallback &o) noexcept;
    void operator=(McCppAsyncCallback &&o) noexcept;

    void call(const QVariantList &varList) noexcept override;

    template<typename Func>
    McCppAsyncCallback(const typename QtPrivate::FunctionPointer<Func>::Object *recever,
                       Func callback) noexcept
        : McCppAsyncCallback()
    {
        typedef QtPrivate::FunctionPointer<Func> FuncType;

        init(QVariantSelector<typename FuncType::Arguments>::isQVariants(),
             recever,
             new QtPrivate::QSlotObject<Func,
                                        typename FuncType::Arguments,
                                        typename FuncType::ReturnType>(callback));
    }
    template<typename Func>
    McCppAsyncCallback(Func callback) noexcept : McCppAsyncCallback()
    {
        init(callback);
    }

    template<typename Func>
    static McCppAsyncCallbackPtr build(
        const typename QtPrivate::FunctionPointer<Func>::Object *recever, Func callback) noexcept
    {
        return McCppAsyncCallbackPtr(new McCppAsyncCallback(recever, callback), McCallbackDeleter());
    }
    template<typename Func>
    static McCppAsyncCallbackPtr build(Func callback) noexcept
    {
        return McCppAsyncCallbackPtr(new McCppAsyncCallback(callback), McCallbackDeleter());
    }

private:
    void init(const QList<bool> &isQVariants,
              const QObject *recever,
              QtPrivate::QSlotObjectBase *callback) noexcept;
    template<typename Func>
    typename std::enable_if<int(QtPrivate::FunctionPointer<Func>::ArgumentCount) >= 0
                                && !QtPrivate::FunctionPointer<Func>::IsPointerToMemberFunction,
                            void>::type
    init(Func callback) noexcept
    {
        typedef QtPrivate::FunctionPointer<Func> FuncType;

        init(QVariantSelector<typename FuncType::Arguments>::isQVariants(),
             nullptr,
             new QtPrivate::QStaticSlotObject<Func,
                                              typename FuncType::Arguments,
                                              typename FuncType::ReturnType>(callback));
    }
    template<typename Func>
    typename std::enable_if<QtPrivate::FunctionPointer<Func>::ArgumentCount == -1, void>::type init(
        Func callback) noexcept
    {
        typedef McPrivate::LambdaType<Func> FuncType;

        init(QVariantSelector<typename FuncType::Arguments>::isQVariants(),
             nullptr,
             new QtPrivate::QFunctorSlotObject<Func,
                                               int(FuncType::ArgumentCount),
                                               typename FuncType::Arguments,
                                               typename FuncType::ReturnType>(std::move(callback)));
    }

private:
    MC_DECL_PRIVATE(McCppAsyncCallback)
};

Q_DECLARE_METATYPE(McCppAsyncCallback)
MC_DECL_METATYPE(McCppAsyncCallback)
