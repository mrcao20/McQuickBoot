#pragma once

#include <McIoc/McGlobal.h>

class IMcCallback
{
public:
    struct McCallbackDeleter
    {
        void operator()(IMcCallback *ptr) { ptr->destroy(); }
    };

    virtual ~IMcCallback() = default;

    virtual void destroy() noexcept = 0;
    virtual void call(const QVariantList &varList) noexcept = 0;

    void call(const QVariant &var) noexcept { call(QVariantList() << var); }

    template<typename... Args>
    void call(const Args &... args) noexcept
    {
        QVariantList vars;
        (vars << ... << toQVariant(args));
        return call(vars);
    }

    void operator()(const QVariantList &varList) noexcept { call(varList); }

    void operator()(const QVariant &var) noexcept { call(var); }

    template<typename... Args>
    void operator()(const Args &... args) noexcept
    {
        call(args...);
    }

private:
    template<typename T>
    typename std::enable_if<McPrivate::IsQVariantHelper<T>::Value, QVariant>::type toQVariant(
        const T &t)
    {
        return QVariant::fromValue(t);
    }
    template<>
    QVariant toQVariant(const QVariant &t)
    {
        return t;
    }
    template<typename T>
    typename std::enable_if<McPrivate::IsQVariantHelper<T>::Value2, QVariant>::type toQVariant(
        const T &t)
    {
        return QVariant(t);
    }
};

MC_DECL_METATYPE(IMcCallback)
