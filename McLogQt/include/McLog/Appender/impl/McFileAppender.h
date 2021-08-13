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

#include "McFileDeviceAppender.h"

MC_FORWARD_DECL_PRIVATE_DATA(McFileAppender);

class MCLOGQT_EXPORT McFileAppender : public McFileDeviceAppender 
{
    Q_OBJECT
    MC_DECL_SUPER(McFileDeviceAppender)
    MC_DECL_INIT(McFileAppender)
    MC_TYPELIST(McFileDeviceAppender)
    Q_PROPERTY(QString dirPath READ dirPath WRITE setDirPath)
    Q_PROPERTY(QString fileNamePattern READ fileNamePattern WRITE setFileNamePattern)
    Q_PROPERTY(bool append READ isAppend WRITE setAppend)
public:
    Q_INVOKABLE McFileAppender();
    ~McFileAppender() override;
    
    QString dirPath() const noexcept;
    void setDirPath(const QString &val) noexcept;
    
    QString fileNamePattern() const noexcept;
    void setFileNamePattern(const QString &val) noexcept;
    
    bool isAppend() const noexcept;
    void setAppend(bool val) noexcept;

protected:
    void doFinished() noexcept override;
    void writeBefore() noexcept override;

protected:
    QString newFilePath() const noexcept;

private:
    void checkExistsFile() noexcept;

private:
    MC_DECL_PRIVATE(McFileAppender)
};

MC_DECL_METATYPE(McFileAppender)
