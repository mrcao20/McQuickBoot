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

#include <McIoc/McIocGlobal.h>

#include "../McLogGlobal.h"
#include "../Repository/IMcAdditionalTask.h"

MC_FORWARD_DECL_PRIVATE_DATA(McLogPackager)

class MC_LOG_EXPORT McLogPackager
    : public QObject
    , public IMcAdditionalTask
{
    Q_OBJECT
    MC_FULL_DEFINE(McLogPackager, QObject, IMcAdditionalTask)
    Q_PRIVATE_PROPERTY(d, QList<QString> scanPaths MEMBER scanPaths)
    Q_PRIVATE_PROPERTY(d, int maxDepth MEMBER maxDepth)
    Q_PRIVATE_PROPERTY(d, QString age MEMBER age)
    Q_PRIVATE_PROPERTY(d, QList<QString> filters MEMBER filters)
    Q_PRIVATE_PROPERTY(d, QString dstPath MEMBER dstPath)
    Q_PRIVATE_PROPERTY(d, QString fileNamePattern MEMBER fileNamePattern)
public:
    McLogPackager() noexcept;
    ~McLogPackager();

    void execute() noexcept override;

private:
    Q_INVOKABLE
    MC_FINISHED
    void buildFinished() noexcept;

    QStringList checkFiles(int depth, const QString &path) noexcept;
    bool fileNameCheck(const QString &fileName) noexcept;
    QString newFilePath() const noexcept;

private:
    MC_DECL_PRIVATE(McLogPackager)
};

MC_DECL_POINTER(McLogPackager)
