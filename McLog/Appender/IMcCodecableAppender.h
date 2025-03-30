/*
 * Copyright (c) 2021 mrcao20/mrcao20@163.com
 * McQuickBoot is licensed under Mulan PSL v2.
 * You can use this software according to the terms and conditions of the Mulan PSL v2.
 * You may obtain a copy of Mulan PSL v2 at:
 *          http://license.coscl.org.cn/MulanPSL2
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND,
 * EITHER EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT,
 * MERCHANTABILITY OR FIT FOR A PARTICULAR PURPOSE.
 * See the Mulan PSL v2 for more details.
 */
#pragma once

#include "../McLogGlobal.h"

#ifdef MC_USE_QT5
QT_BEGIN_NAMESPACE
class QTextCodec;
QT_END_NAMESPACE
#else
# include <QStringConverter>
#endif

class IMcCodecableAppender
{
    MC_DEFINE_INTERFACE(IMcCodecableAppender)
public:
#ifdef MC_USE_QT5
    virtual QTextCodec *codec() const noexcept = 0;
    virtual void setCodec(QTextCodec *val) noexcept = 0;
#else
    virtual QStringConverter::Encoding encoding() const noexcept = 0;
    virtual void setEncoding(QStringConverter::Encoding val) noexcept = 0;
#endif
};

MC_DECL_METATYPE(IMcCodecableAppender)
