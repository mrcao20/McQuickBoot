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
#include "McAbstractFormatAppender.h"

#include <QCoreApplication>
#include <QLockFile>
#include <QScopeGuard>
#include <QThread>
#include <qlogging.h>

#include "Layout/Impl/McNormalLayout.h"

MC_DECL_PRIVATE_DATA(McAbstractFormatAppender)
IMcLayoutPtr layout;
bool immediateFlush{false}; //!< 是否立即刷新输出，默认为false
bool useLockFile{false}; //!< 是否使用文件锁，如果程序允许同时运行多个，则需要此功能
bool isPrintError{true}; //!< 是否打印因没有定义QT_MESSAGELOGCONTEXT宏导致的错误消息
QString lockFilePath{"./.mcLog/~.lockFile"};
QScopedPointer<QLockFile> lockFile;
MC_DECL_PRIVATE_DATA_END

McAbstractFormatAppender::McAbstractFormatAppender() noexcept
{
    MC_NEW_PRIVATE_DATA(McAbstractFormatAppender);
}

McAbstractFormatAppender::~McAbstractFormatAppender() {}

IMcLayoutPtr McAbstractFormatAppender::layout() const noexcept
{
    return d->layout;
}

void McAbstractFormatAppender::setLayout(const IMcLayoutPtr &val) noexcept
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

bool McAbstractFormatAppender::isPrintError() const noexcept
{
    return d->isPrintError;
}

void McAbstractFormatAppender::setPrintError(bool val) noexcept
{
    d->isPrintError = val;
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
    if (!types().contains(type)) {
        return;
    }

    auto l = layout();
    if (l.isNull()) {
        MC_PRINT_ERR("the appender not set layout. category: %s\n", context.category);
        return;
    }
    //! 如果没有定义QT_MESSAGELOGCONTEXT
    if (context.file == nullptr && context.line == 0 && context.function == nullptr && d->isPrintError) {
        MC_PRINT_ERR("in release, need to manual define QT_MESSAGELOGCONTEXT\n");
    }
    auto message = l->format(type, context, str);

    auto e = new McCustomEvent(QEvent::User, message);
    qApp->postEvent(this, e);
}

void McAbstractFormatAppender::buildFinished() noexcept
{
    super::buildFinished();

    if (layout().isNull()) {
        auto l = McNormalLayoutPtr::create();
        l->buildStarted();
        l->buildFinished();
        l->buildThreadMoved();
        l->buildCompleted();
        setLayout(l);
    }
}

void McAbstractFormatAppender::buildThreadMoved() noexcept
{
    super::buildThreadMoved();

    auto l = layout(); //!< 一定存在
    auto pl = l.staticCast<McPatternLayout>(); //!< 一定成功
    if (pl->thread() != thread()) {
        pl->moveToThread(thread());
    }
}

void McAbstractFormatAppender::buildCompleted() noexcept
{
    super::buildCompleted();

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
    if (event->type() == QEvent::User) {
        auto e = static_cast<McCustomEvent *>(event);
        append_helper(e->data().toString());
    }
}

void McAbstractFormatAppender::append_helper(const QString &msg) noexcept
{
    auto cleanup = qScopeGuard([this]() {
        if (!d->useLockFile) {
            return;
        }
        d->lockFile->unlock();
    });
    if (d->useLockFile) {
        if (!d->lockFile->lock()) {
            qCCritical(mcLog()) << "cannot use lock file for path:" << d->lockFilePath;
        }
    }
    writeBefore();
    auto out = device();
    if (out.isNull() || !out->isOpen()) {
        return;
    }
    if (d->useLockFile && !out->isSequential()) {
        out->seek(out->size());
    }
    textStream() << msg;
    if (d->immediateFlush || d->useLockFile) {
        textStream() << Qt::endl;
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
