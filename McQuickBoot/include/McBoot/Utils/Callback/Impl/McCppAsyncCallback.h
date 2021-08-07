/*
 * MIT License
 *
 * Copyright (c) 2021 mrcao20
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */
#pragma once

#include "McAbstractAsyncCallback.h"

MC_FORWARD_DECL_PRIVATE_DATA(McCppAsyncCallback);

class MCQUICKBOOT_EXPORT McCppAsyncCallback : public McAbstractAsyncCallback
{
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
    McCppAsyncCallback() noexcept;
    ~McCppAsyncCallback() override;

    McCppAsyncCallback(const McCppAsyncCallback &o) noexcept;
    McCppAsyncCallback(McCppAsyncCallback &&o) noexcept;
    McCppAsyncCallback &operator=(const McCppAsyncCallback &o) noexcept;
    McCppAsyncCallback &operator=(McCppAsyncCallback &&o) noexcept;

    void call(const QVariantList &varList) noexcept override;

    template<typename Func>
    McCppAsyncCallback(const typename QtPrivate::FunctionPointer<Func>::Object *recever,
                       Func callback) noexcept
        : McCppAsyncCallback()
    {
        typedef QtPrivate::FunctionPointer<Func> FuncType;

        init(QVariantSelector<typename FuncType::Arguments>::isQVariants(),
             QMetaTypeIdSelector<typename FuncType::Arguments>::metaTypeIds(),
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
              const QList<int> &argumentIds,
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
    MC_DECL_PRIVATE(McCppAsyncCallback)
};

Q_DECLARE_METATYPE(McCppAsyncCallback)
MC_DECL_METATYPE(McCppAsyncCallback)
