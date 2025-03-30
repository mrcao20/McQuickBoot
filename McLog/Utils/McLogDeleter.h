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

#include <McIoc/McIocGlobal.h>

#include "../McLogGlobal.h"
#include "../Repository/IMcAdditionalTask.h"

MC_FORWARD_DECL_PRIVATE_DATA(McLogDeleter)

class MC_LOG_EXPORT McLogDeleter
    : public QObject
    , public IMcAdditionalTask
{
    Q_OBJECT
    MC_FULL_DEFINE(McLogDeleter, QObject, IMcAdditionalTask)
    Q_PRIVATE_PROPERTY(d, QList<QString> basePaths MEMBER basePaths)
    Q_PRIVATE_PROPERTY(d, int maxDepth MEMBER maxDepth)
    Q_PRIVATE_PROPERTY(d, QString age MEMBER age)
    Q_PRIVATE_PROPERTY(d, QList<QString> filters MEMBER filters)
public:
    McLogDeleter() noexcept;
    ~McLogDeleter();

    void execute() noexcept override;

private:
    Q_INVOKABLE
    MC_FINISHED
    void buildFinished() noexcept;

    void checkFiles(int depth, const QString &path) noexcept;
    bool fileNameCheck(const QString &fileName) noexcept;

private:
    MC_DECL_PRIVATE(McLogDeleter)
};

MC_DECL_METATYPE(McLogDeleter)
