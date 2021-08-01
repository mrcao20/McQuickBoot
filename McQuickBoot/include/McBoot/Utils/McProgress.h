#pragma once

#include <QObject>
#include <QSharedData>

#include "Callback/Impl/McCppSyncCallback.h"

struct MCQUICKBOOT_EXPORT McProgressSharedData : public QSharedData
{
    QAtomicInt current{0};
    QAtomicInt total{100};
    IMcCallbackPtr callback;
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
                          "The number of parameters of callback function can only be equal to "
                          "2(int current, int total)");

        setCallback(McCppSyncCallback::build(recever, func));
    }
    template<typename Func>
    typename std::enable_if<int(QtPrivate::FunctionPointer<Func>::ArgumentCount) >= 0
                                && !QtPrivate::FunctionPointer<Func>::IsPointerToMemberFunction,
                            void>::type
    callback(Func func) noexcept
    {
        typedef QtPrivate::FunctionPointer<Func> FuncType;

        Q_STATIC_ASSERT_X(int(FuncType::ArgumentCount) == 2,
                          "The number of parameters of callback function can only be equal to "
                          "2(int current, int total)");

        setCallback(McCppSyncCallback::build(func));
    }
    template<typename Func>
    typename std::enable_if<QtPrivate::FunctionPointer<Func>::ArgumentCount == -1, void>::type
    callback(Func func) noexcept
    {
        typedef McPrivate::LambdaType<Func> FuncType;

        Q_STATIC_ASSERT_X(int(FuncType::ArgumentCount) == 2,
                          "The number of parameters of callback function can only be equal to "
                          "2(int current, int total)");

        setCallback(McCppSyncCallback::build(func));
    }

private:
    void setCallback(const IMcCallbackPtr &val) noexcept;
    void callCallback() noexcept;

private:
    QExplicitlySharedDataPointer<McProgressSharedData> d;
};

Q_DECLARE_METATYPE(McProgress)
