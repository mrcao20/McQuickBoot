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
#include "McLog/Appender/impl/McAbstractFormatAppender.h"

#include <QCoreApplication>
#include <QLockFile>
#include <QScopeGuard>
#include <QThread>
#include <qlogging.h>

#include "McLog/Layout/impl/McNormalLayout.h"

MC_DECL_PRIVATE_DATA(McAbstractFormatAppender)
IMcLayoutPtr layout;
bool immediateFlush{false}; //!< 是否立即刷新输出，默认为false
bool useLockFile{false}; //!< 是否使用文件锁，如果程序允许同时运行多个，则需要此功能
QString lockFilePath{"./.mcLogQt/~.lockFile"};
QScopedPointer<QLockFile> lockFile;
MC_DECL_PRIVATE_DATA_END

MC_INIT(McAbstractFormatAppender)
MC_INIT_END

McAbstractFormatAppender::McAbstractFormatAppender() 
{
    MC_NEW_PRIVATE_DATA(McAbstractFormatAppender);
}

McAbstractFormatAppender::~McAbstractFormatAppender() 
{
}

IMcLayoutPtr McAbstractFormatAppender::layout() const noexcept 
{
    return d->layout;
}

void McAbstractFormatAppender::setLayout(IMcLayoutConstPtrRef val) noexcept 
{
    d->layout = val;
}

bool McAbstractFormatAppender::immediateFlush() const noexcept 
{
    return d->immediateFlush;
}

void McAbstractFormatAppender::setImmediateFlush(bool val) noexcept 
{
    d->immediateFlush = val;
}

bool McAbstractFormatAppender::useLockFile() const noexcept
{
    return d->useLockFile;
}

void McAbstractFormatAppender::setUseLockFile(bool val) noexcept
{
    d->useLockFile = val;
}

QString McAbstractFormatAppender::lockFilePath() const noexcept
{
    return d->lockFilePath;
}

void McAbstractFormatAppender::setLockFilePath(const QString &val) noexcept
{
    d->lockFilePath = val;
}

void McAbstractFormatAppender::append(QtMsgType type, const QMessageLogContext &context, const QString &str) noexcept 
{
    if(!types().contains(type)) {
        return;
    }
    
    auto l = layout();
    if(l.isNull()) {
        MC_PRINT_ERR("the appender not set layout. category: %s\n", context.category);
        return;
    }
    //! 如果没有定义QT_MESSAGELOGCONTEXT
    if (context.file == nullptr
        && context.line == 0
        && context.function == nullptr) {
        MC_PRINT_ERR("in release, need to manual define QT_MESSAGELOGCONTEXT\n");
    }
    auto message = l->format(type, context, str);

    auto e = new McCustomEvent(QEvent::User, message);
    qApp->postEvent(this, e);
}

void McAbstractFormatAppender::doFinished() noexcept
{
    McAbstractIODeviceAppender::doFinished();

    if (layout().isNull()) {
        auto l = McNormalLayoutPtr::create();
        l->finished();
        setLayout(l);
    }
}

void McAbstractFormatAppender::doThreadFinished() noexcept
{
    McAbstractIODeviceAppender::doThreadFinished();

    auto l = layout();  //!< 一定存在
    auto pl = l.staticCast<McPatternLayout>();  //!< 一定成功
    if(pl->thread() != thread()) {
        pl->moveToThread(thread());
    }
}

void McAbstractFormatAppender::doAllFinished() noexcept
{
    super::doAllFinished();

    if (!d->lockFilePath.isEmpty() && d->useLockFile) {
        auto filePath = Mc::toAbsolutePath(d->lockFilePath);
        QFileInfo fileInfo(filePath);
        auto dir = fileInfo.absoluteDir();
        if (!dir.exists()) {
            dir.mkpath(dir.absolutePath());
        }
        d->lockFile.reset(new QLockFile(filePath));
    }
}

void McAbstractFormatAppender::customEvent(QEvent *event) 
{
    if(event->type() == QEvent::User) {
        auto e = static_cast<McCustomEvent *>(event);
        append_helper(e->data().toString());
    }
}

void McAbstractFormatAppender::append_helper(const QString &msg) noexcept
{
    if (d->useLockFile) {
        if (!d->lockFile->lock()) {
            qCritical() << "cannot use lock file for path:" << d->lockFilePath;
        }
        auto cleanup = qScopeGuard([this]() { d->lockFile->unlock(); });
    }
    writeBefore();
    auto out = device();
    if(out.isNull() || !out->isOpen()) {
        return;
    }
    if (d->useLockFile && !out->isSequential()) {
        out->seek(out->size());
    }
    textStream() << msg;
    if (d->immediateFlush || d->useLockFile) {
#if QT_VERSION < QT_VERSION_CHECK(5, 15, 0)
        textStream() << endl;
#else
        textStream() << Qt::endl;
#endif
    } else {
#ifdef Q_OS_WIN
        if (out->openMode().testFlag(QIODevice::Text)) {
            textStream() << "\r\n";
        } else {
            textStream() << "\n";
        }
#else
        textStream() << "\n";
#endif
    }
    writeAfter();
}
