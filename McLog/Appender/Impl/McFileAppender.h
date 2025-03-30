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

MC_FORWARD_DECL_PRIVATE_DATA(McFileAppender)

class MC_LOG_EXPORT McFileAppender : public McAbstractFormatAppender
{
    Q_OBJECT
    MC_FULL_DEFINE(McFileAppender, McAbstractFormatAppender)
    Q_PROPERTY(QString dirPath READ dirPath WRITE setDirPath)
    Q_PROPERTY(QString fileNamePattern READ fileNamePattern WRITE setFileNamePattern)
    Q_PROPERTY(bool append READ isAppend WRITE setAppend)
public:
    McFileAppender() noexcept;
    ~McFileAppender();

    QString dirPath() const noexcept;
    void setDirPath(const QString &val) noexcept;

    QString fileNamePattern() const noexcept;
    void setFileNamePattern(const QString &val) noexcept;

    bool isAppend() const noexcept;
    void setAppend(bool val) noexcept;

    void buildFinished() noexcept override;

protected:
    void writeBefore() noexcept override;
    void writeAfter() noexcept override;

    QString newFilePath() const noexcept;
    bool checkExistsFile() noexcept;
    QString getLastFilePath() const noexcept;

private:
    MC_DECL_PRIVATE(McFileAppender)
};

MC_DECL_METATYPE(McFileAppender)
