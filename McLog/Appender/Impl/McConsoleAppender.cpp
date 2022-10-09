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
