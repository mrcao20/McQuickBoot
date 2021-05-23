#pragma once

#include "McAbstractSyncCallback.h"

MC_FORWARD_DECL_PRIVATE_DATA(McCppSyncCallback);

class MCQUICKBOOT_EXPORT McCppSyncCallback : public McAbstractSyncCallback
{
    Q_OBJECT
    MC_TYPELIST(McAbstractSyncCallback)
    MC_DECL_POINTER(McCppSyncCallback)

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

    template<typename T>
    struct QMetaTypeIdSelector;
    template<typename... Args>
    struct QMetaTypeIdSelector<QtPrivate::List<Args...>>
    {
        static QList<int> metaTypeIds()
        {
            QList<int> l;
            (l << ... << qMetaTypeId<Args>());
            return l;
        }
    };

public:
    McCppSyncCallback() noexcept;
    ~McCppSyncCallback() override;

    McCppSyncCallback(const McCppSyncCallback &o) noexcept;
    McCppSyncCallback(McCppSyncCallback &&o) noexcept;
    McCppSyncCallback &operator=(const McCppSyncCallback &o) noexcept;
    McCppSyncCallback &operator=(McCppSyncCallback &&o) noexcept;

    template<typename Func>
    McCppSyncCallback(const typename QtPrivate::FunctionPointer<Func>::Object *receiver,
                      Func callback) noexcept
        : McCppSyncCallback()
    {
        typedef QtPrivate::FunctionPointer<Func> FuncType;

        init(QVariantSelector<typename FuncType::Arguments>::isQVariants(),
             QMetaTypeIdSelector<typename FuncType::Arguments>::metaTypeIds(),
             receiver,
             new QtPrivate::QSlotObject<Func,
                                        typename FuncType::Arguments,
                                        typename FuncType::ReturnType>(callback));
    }
    template<typename Func>
    McCppSyncCallback(Func callback) noexcept : McCppSyncCallback()
    {
        init(callback);
    }

    template<typename Func>
    static McCppSyncCallbackPtr build(
        const typename QtPrivate::FunctionPointer<Func>::Object *receiver, Func callback) noexcept
    {
        return McCppSyncCallbackPtr(new McCppSyncCallback(receiver, callback), McCallbackDeleter());
    }
    template<typename Func>
    static McCppSyncCallbackPtr build(Func callback) noexcept
    {
        return McCppSyncCallbackPtr(new McCppSyncCallback(callback), McCallbackDeleter());
    }

protected:
    void syncCall(const QVariantList &varList) noexcept override;

private:
    void init(const QList<bool> &isQVariants,
              const QList<int> &argumentIds,
              const QObject *receiver,
              QtPrivate::QSlotObjectBase *callback) noexcept;
    template<typename Func>
    typename std::enable_if<int(QtPrivate::FunctionPointer<Func>::ArgumentCount) >= 0
                                && !QtPrivate::FunctionPointer<Func>::IsPointerToMemberFunction,
                            void>::type
    init(Func callback) noexcept
    {
        typedef QtPrivate::FunctionPointer<Func> FuncType;

        init(QVariantSelector<typename FuncType::Arguments>::isQVariants(),
             QMetaTypeIdSelector<typename FuncType::Arguments>::metaTypeIds(),
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
             QMetaTypeIdSelector<typename FuncType::Arguments>::metaTypeIds(),
             nullptr,
             new QtPrivate::QFunctorSlotObject<Func,
                                               int(FuncType::ArgumentCount),
                                               typename FuncType::Arguments,
                                               typename FuncType::ReturnType>(std::move(callback)));
    }

private:
    MC_DECL_PRIVATE(McCppSyncCallback)
};

Q_DECLARE_METATYPE(McCppSyncCallback)
MC_DECL_METATYPE(McCppSyncCallback)
