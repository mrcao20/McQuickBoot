#include "McLog/Device/McVSDebugDevice.h"

#include <mutex>

#include <QMutex>

#ifdef Q_OS_WIN
#include <qt_windows.h>
#endif

MC_INIT(McVSDebugDevice)
MC_REGISTER_BEAN_FACTORY(McVSDebugDevice)
MC_INIT_END

MC_DECL_PRIVATE_DATA(McVSDebugDevice)
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

McVSDebugDevice::McVSDebugDevice(QObject *parent) noexcept
    : QIODevice(parent)
{
    MC_NEW_PRIVATE_DATA(McVSDebugDevice);
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
    QString msg = QString::fromLocal8Bit(data, maxSize);
#ifdef Q_OS_WIN
    win_outputDebugString_helper(msg);
#endif
    return maxSize;
}
