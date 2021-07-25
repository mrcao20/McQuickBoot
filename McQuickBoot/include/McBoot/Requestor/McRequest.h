#pragma once

#include <QSharedData>

#include "../McBootGlobal.h"
#include "../Utils/McCancel.h"
#include "../Utils/McProgress.h"

struct MCQUICKBOOT_EXPORT McRequestSharedData : public QSharedData
{
    McCancel cancel;
    McProgress progress;
    QVariantList params;
};

class MCQUICKBOOT_EXPORT McRequest
{
    MC_DECL_INIT(McRequest)
public:
    McRequest() noexcept;
    ~McRequest();

    McCancel cancel() const noexcept;
    McProgress progress() const noexcept;

    int count() const noexcept;
    QVariant variant(int i) const noexcept;
    template<typename T>
    T at(int i) const noexcept
    {
        QVariant var = variant(i);
        return var.value<T>();
    }
    template<typename... Args>
    bool check() const noexcept
    {
        auto argSize = sizeof...(Args);
        if (argSize != d->params.size()) {
            return false;
        }
        return CheckHelper<Args...>::check(0, d->params);
    }

private:
    void setCancel(const McCancel &val) noexcept;
    void setProgress(const McProgress &val) noexcept;
    void setParams(const QVariantList &val) noexcept;
    template<typename...>
    struct CheckHelper;
    template<typename T, typename... Args>
    struct CheckHelper<T, Args...>
    {
        static bool check(int index, const QVariantList &params) noexcept
        {
            auto param = params.at(index);
            if (!param.canConvert<T>()) {
                return false;
            }
            return CheckHelper<Args...>::check(index + 1, params);
        }
    };
    template<>
    struct CheckHelper<>
    {
        static bool check(int, const QVariantList &) noexcept { return true; }
    };

private:
    QExplicitlySharedDataPointer<McRequestSharedData> d;

    friend class McRequestRunner;
    friend class McControllerContainer;
};

Q_DECLARE_METATYPE(McRequest)
