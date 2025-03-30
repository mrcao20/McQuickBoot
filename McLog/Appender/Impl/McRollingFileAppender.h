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

#include "../IMcRequestableNextFile.h"
#include "McFileAppender.h"

MC_FORWARD_DECL_PRIVATE_DATA(McRollingFileAppender)

class MC_LOG_EXPORT McRollingFileAppender
    : public McFileAppender
    , public IMcRequestableNextFile
{
    Q_OBJECT
    MC_DECL_SUPER(McFileAppender)
    MC_INTERFACES(McFileAppender, IMcRequestableNextFile)
    Q_PROPERTY(QString backupDirPath READ backupDirPath WRITE setBackupDirPath)
    Q_PROPERTY(QString backupDirPattern READ backupDirPattern WRITE setBackupDirPattern)
public:
    McRollingFileAppender() noexcept;
    ~McRollingFileAppender();

    QString backupDirPath() const noexcept;
    void setBackupDirPath(const QString &val) noexcept;

    QString backupDirPattern() const noexcept;
    void setBackupDirPattern(const QString &val) noexcept;

    void requestNextFile() noexcept override;
    bool forceRequestNextFile() noexcept override;

    void buildFinished() noexcept override;

protected:
    void writeBefore() noexcept override;
    void writeAfter() noexcept override;

    virtual bool isRolling() noexcept = 0;

    void tryNextFile() noexcept;

private:
    bool nextFile() noexcept;
    QString newBackupPath(const QString &oldFilePath) const noexcept;

private:
    MC_DECL_PRIVATE(McRollingFileAppender)
};

MC_DECL_METATYPE(McRollingFileAppender)
