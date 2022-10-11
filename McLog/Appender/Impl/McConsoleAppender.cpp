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
#include "McConsoleAppender.h"

#include <QFile>
#ifdef MC_USE_QT5
# include <QTextCodec>
#endif

MC_AUTO_INIT(McConsoleAppender)
MC_INIT_END

MC_DECL_PRIVATE_DATA(McConsoleAppender)
QString console;
MC_DECL_PRIVATE_DATA_END

McConsoleAppender::McConsoleAppender() noexcept
{
    MC_NEW_PRIVATE_DATA(McConsoleAppender);
}

McConsoleAppender::~McConsoleAppender() {}

QString McConsoleAppender::console() const noexcept
{
    return d->console;
}

void McConsoleAppender::setConsole(const QString &val) noexcept
{
    d->console = val;
}

void McConsoleAppender::buildFinished() noexcept
{
    super::buildFinished();

    QSharedPointer<QFile> file = QSharedPointer<QFile>::create();

    FILE *cFile = stderr;
    if (d->console == QLatin1String("stdout")) {
        cFile = stdout;
    }
    if (!file->open(cFile, QIODevice::WriteOnly)) {
        MC_PRINT_ERR("error open stdout for write console!!!\n");
        return;
    }

    setDevice(file);
#ifdef MC_USE_QT5
    setCodec(QTextCodec::codecForLocale());
#else
    setEncoding(QStringConverter::System);
#endif
}

void McConsoleAppender::writeBefore() noexcept {}

void McConsoleAppender::writeAfter() noexcept {}
