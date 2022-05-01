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

#include <McIoc/BeanBuilder/IMcCustomPlaceholder.h>
#include <McIoc/McIocGlobal.h>
#include <McIoc/Utils/IMcBeanBuildable.h>

#include "../CoreExample/SimpleInterface.h"

class SimpleObject
    : public QObject
    , public SimpleInterface
    , public IMcBeanBuildable
    , public IMcCustomPlaceholder
{
    Q_OBJECT
    Q_INTERFACES(SimpleInterface)
    MC_FULL_DEFINE(SimpleObject, QObject, SimpleInterface, IMcCustomPlaceholder)
public:
    void simpleFunc() override;

    void buildStarted() noexcept override;
    void buildFinished() noexcept override;
    void buildThreadMoved() noexcept override;
    void buildCompleted() noexcept override;

    QVariant getKey() const noexcept override;
};

MC_DECL_POINTER(SimpleObject)
