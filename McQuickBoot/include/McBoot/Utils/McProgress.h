#pragma once

#include <QObject>
#include <QSharedData>

#include <McIoc/McGlobal.h>

#include "../McBootMacroGlobal.h"

struct MCQUICKBOOT_EXPORT McProgressSharedData : public QSharedData
{
    McProgressSharedData() {}
    McProgressSharedData(const McProgressSharedData &o)
        : QSharedData(o), current(o.current), total(o.total), recever(o.recever),
          attached(o.attached)
    {
        auto c = callback.loadAcquire();
        if (c != nullptr) {
            c->destroyIfLastRef();
            callback.storeRelease(nullptr);
        }
        auto oc = o.callback.loadAcquire();
        if (oc != nullptr) {
            oc->ref();
            callback.storeRelease(oc);
        }
    }
    ~McProgressSharedData()
    {
        auto c = callback.loadAcquire();
        if (c != nullptr) {
            c->destroyIfLastRef();
            callback.storeRelease(nullptr);
        }
    }
    QAtomicInt current{0};
    QAtomicInt total{100};
    QAtomicPointer<QObject> recever{nullptr};
    QAtomicPointer<QtPrivate::QSlotObjectBase> callback{nullptr};
    QSharedPointer<QObject> attached;
};

class MCQUICKBOOT_EXPORT McProgress : public QObject
{
    Q_OBJECT
    MC_DECL_INIT(McProgress)
public:
    McProgress() noexcept;
    ~McProgress();
    McProgress(const McProgress &o) noexcept;
    McProgress(McProgress &&o) noexcept;
    McProgress &operator=(const McProgress &o) noexcept;
    McProgress &operator=(McProgress &&o) noexcept;

    int current() const noexcept;
    void setCurrent(int val) noexcept;
    int total() const noexcept;
    void setTotal(int val) noexcept;

    template<typename Func>
    void callback(const typename QtPrivate::FunctionPointer<Func>::Object *recever,
                  Func func) noexcept
    {
        typedef QtPrivate::FunctionPointer<Func> FuncType;

        Q_STATIC_ASSERT_X(int(FuncType::ArgumentCount) == 2,
                          "The number of parameters of callback function can only be equal to 2");

        setCallback(recever,
                    new QtPrivate::QSlotObject<Func,
                                               typename FuncType::Arguments,
                                               typename FuncType::ReturnType>(func));
    }
    template<typename Func>
    typename std::enable_if<int(QtPrivate::FunctionPointer<Func>::ArgumentCount) >= 0
                                && !QtPrivate::FunctionPointer<Func>::IsPointerToMemberFunction,
                            void>::type
    callback(Func func) noexcept
    {
        typedef QtPrivate::FunctionPointer<Func> FuncType;

        Q_STATIC_ASSERT_X(int(FuncType::ArgumentCount) == 2,
                          "The number of parameters of callback function can only be equal to 2");

        setCallback(nullptr,
                    new QtPrivate::QStaticSlotObject<Func,
                                                     typename FuncType::Arguments,
                                                     typename FuncType::ReturnType>(func));
    }
    template<typename Func>
    typename std::enable_if<QtPrivate::FunctionPointer<Func>::ArgumentCount == -1, void>::type
    callback(Func func) noexcept
    {
        typedef McPrivate::LambdaType<Func> FuncType;

        Q_STATIC_ASSERT_X(int(FuncType::ArgumentCount) == 2,
                          "The number of parameters of callback function can only be equal to 2");

        setCallback(nullptr,
                    new QtPrivate::QFunctorSlotObject<Func,
                                                      int(FuncType::ArgumentCount),
                                                      typename FuncType::Arguments,
                                                      typename FuncType::ReturnType>(
                        std::move(func)));
    }

private:
    void setCallback(const QObject *recever, QtPrivate::QSlotObjectBase *callback) noexcept;
    void callCallback() noexcept;

private:
    QExplicitlySharedDataPointer<McProgressSharedData> d;
};

Q_DECLARE_METATYPE(McProgress)
