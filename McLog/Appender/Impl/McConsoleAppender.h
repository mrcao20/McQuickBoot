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

#include "McAbstractFormatAppender.h"

MC_FORWARD_DECL_PRIVATE_DATA(McConsoleAppender)

class MC_LOG_EXPORT McConsoleAppender : public McAbstractFormatAppender
{
    Q_OBJECT
    MC_FULL_DEFINE(McConsoleAppender, McAbstractFormatAppender)
    Q_PROPERTY(QString console READ console WRITE setConsole)
public:
    McConsoleAppender() noexcept;
    ~McConsoleAppender();

    QString console() const noexcept;
    void setConsole(const QString &val) noexcept;

    void buildFinished() noexcept override;

protected:
    void writeBefore() noexcept override;
    void writeAfter() noexcept override;

private:
    MC_DECL_PRIVATE(McConsoleAppender)
};

MC_DECL_METATYPE(McConsoleAppender)
