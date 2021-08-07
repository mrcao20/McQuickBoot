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
#include "McLog/Device/McVSDebugDevice.h"

#include <mutex>

#include <QMutex>
#include <QTextCodec>

#include "McLog/Appender/IMcCodecableAppender.h"

#ifdef Q_OS_WIN
#include <qt_windows.h>
#endif

MC_INIT(McVSDebugDevice)
MC_REGISTER_BEAN_FACTORY(McVSDebugDevice)
MC_INIT_END

MC_DECL_PRIVATE_DATA(McVSDebugDevice)
IMcCodecableAppender *codecableAppender{nullptr};
MC_DECL_PRIVATE_DATA_END

namespace {
    
template <typename Mutex, typename Lock = std::unique_lock<typename std::decay<Mutex>::type>>
Lock qt_unique_lock(Mutex &mutex)
{
    return Lock(mutex);
}

template <typename Mutex, typename Lock = std::unique_lock<typename std::decay<Mutex>::type>>
Lock qt_unique_lock(Mutex *mutex)
{
    return mutex ? Lock(*mutex) : Lock() ;
}
    
#ifdef Q_OS_WIN
#if QT_VERSION < QT_VERSION_CHECK(5, 15, 0)
static void win_outputDebugString_helper(const QString &message)
#else
static void win_outputDebugString_helper(QStringView message)
#endif
{
    const int maxOutputStringLength = 32766;
    static QBasicMutex m;
    auto locker = qt_unique_lock(m);
    // fast path: Avoid string copies if one output is enough
    if (message.length() <= maxOutputStringLength) {
        OutputDebugString(reinterpret_cast<const wchar_t *>(message.utf16()));
    } else {
        wchar_t *messagePart = new wchar_t[maxOutputStringLength + 1];
        for (int i = 0; i < message.length(); i += maxOutputStringLength ) {
            const int length = std::min(message.length() - i, maxOutputStringLength );
            const int len = message.mid(i, length).toWCharArray(messagePart);
            Q_ASSERT(len == length);
            messagePart[len] = 0;
            OutputDebugString(messagePart);
        }
        delete[] messagePart;
    }
}
#endif

}

McVSDebugDevice::McVSDebugDevice(IMcCodecableAppender *codecableAppender, QObject *parent) noexcept
    : QIODevice(parent)
{
    MC_NEW_PRIVATE_DATA(McVSDebugDevice);

    d->codecableAppender = codecableAppender;
}

McVSDebugDevice::~McVSDebugDevice()
{
}

qint64 McVSDebugDevice::readData(char *data, qint64 maxSize)
{
    Q_UNUSED(data);
    Q_UNUSED(maxSize);
    return 0;
}

qint64 McVSDebugDevice::writeData(const char *data, qint64 maxSize)
{
    QString msg = d->codecableAppender->codec()->toUnicode(data, static_cast<int>(maxSize));
#ifdef Q_OS_WIN
    win_outputDebugString_helper(msg);
#endif
    return maxSize;
}
