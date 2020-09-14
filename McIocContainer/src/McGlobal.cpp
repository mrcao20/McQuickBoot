#include "McIoc/McGlobal.h"

#include <QEventLoop>
#include <QTimer>
#include <QElapsedTimer>
#include <QDir>
#include <QCoreApplication>
#include <QUrl>

McCustomEvent::~McCustomEvent() noexcept
{
}

namespace Mc {

bool waitForExecFunc(const std::function<bool()> &func, qint64 timeout) noexcept 
{
    QEventLoop loop;
    QTimer timer;
    timer.setInterval(100);
    QElapsedTimer stopWatcher;
    bool ret = true;
    
    QObject::connect(&timer, &QTimer::timeout, [&timer, &stopWatcher, &loop, &ret, &func, &timeout](){
        timer.stop();
        if((ret = func())) {
            loop.quit();
            return;
        }
        if(timeout != -1 && stopWatcher.elapsed() > timeout) {
            loop.quit();
            return;
        }
        timer.start();
    });
    
    stopWatcher.start();
    timer.start();
    loop.exec();
    return ret;
}

QString toAbsolutePath(const QString &path) noexcept
{
    QString dstPath = QDir::toNativeSeparators(path);
    QString sepDot = ".";
    QString sepDotDot = "..";
    sepDot += QDir::separator();
    sepDotDot += QDir::separator();
    if(dstPath.startsWith(sepDot) || dstPath.startsWith(sepDotDot)) {
        dstPath = qApp->applicationDirPath() + QDir::separator() + dstPath;
    } else {
        QUrl url(path);
        if(url.isLocalFile()) {
            dstPath = url.toLocalFile();
            dstPath = QDir::toNativeSeparators(dstPath);
            if(!QDir::isAbsolutePath(dstPath)) {
                dstPath = qApp->applicationDirPath() + QDir::separator() + dstPath;
            }
            url = QUrl::fromLocalFile(dstPath);
        }
        dstPath = url.toString();
    }
    return dstPath;
}

}
