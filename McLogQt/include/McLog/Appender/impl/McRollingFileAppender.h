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

#include "../IMcRequestableNextFile.h"
#include "McFileAppender.h"

MC_FORWARD_DECL_PRIVATE_DATA(McRollingFileAppender);

class MCLOGQT_EXPORT McRollingFileAppender : public McFileAppender, public IMcRequestableNextFile
{
    Q_OBJECT
    MC_DECL_INIT(McRollingFileAppender)
    MC_TYPELIST(McFileAppender, IMcRequestableNextFile)
    Q_PROPERTY(QString backupDirPath READ backupDirPath WRITE setBackupDirPath)
    Q_PROPERTY(QString backupDirPattern READ backupDirPattern WRITE setBackupDirPattern)
public:
    McRollingFileAppender();
    ~McRollingFileAppender() override;
    
    QString backupDirPath() const noexcept;
    void setBackupDirPath(const QString &val) noexcept;
    
    QString backupDirPattern() const noexcept;
    void setBackupDirPattern(const QString &val) noexcept;

    void requestNextFile() noexcept override;
    void forceRequestNextFile() noexcept override;

protected:
    void doFinished() noexcept override;

    void tryNextFile() noexcept override;
    
    virtual bool isNewNextFile() noexcept = 0;
    
private:
    Q_INVOKABLE void nextFile() noexcept;
    QString newBackupPath(const QString &oldFilePath) const noexcept;
    
private:
    MC_DECL_PRIVATE(McRollingFileAppender)
};

MC_DECL_METATYPE(McRollingFileAppender)
