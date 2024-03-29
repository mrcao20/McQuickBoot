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
#include "McBoot/Application/McSingleApplication.h"

#include <QLocalServer>
#include <QLocalSocket>
#include <QFileInfo>

#define TIME_OUT                (500)    // 500ms

MC_DECL_PRIVATE_DATA(McSingleApplication)
QObject *mainApp{nullptr};              //!< 正在运行的实例
QLocalServer *localServer{nullptr};      //!< 本地socket Server
QString serverName;                     //!< 服务名称
bool isRunning{false};                  //!< 是否已经有实例正在运行
MC_PADDING_CLANG(7)
MC_DECL_PRIVATE_DATA_END

McSingleApplication::McSingleApplication(int &argc, char **argv) noexcept
    : QGuiApplication(argc, argv)
{
    MC_NEW_PRIVATE_DATA(McSingleApplication);
#ifndef QT_DEBUG
    //! 取应用程序名作为LocalServer的名字
    d->serverName = QFileInfo(Mc::applicationFilePath()).fileName();
    initLocalConnection();
#endif // !_DEBUG
}

McSingleApplication::~McSingleApplication() noexcept 
{
}

bool McSingleApplication::isRunning() noexcept 
{
    return d->isRunning;
}

void McSingleApplication::setAppObj(QObject *obj) noexcept 
{
    d->mainApp = obj;
}

void McSingleApplication::slot_newLocalConnection() noexcept 
{
    QLocalSocket *socket = d->localServer->nextPendingConnection();
    if (socket) {
        qInfo() << "new app startup. cancel it. active this";
        socket->waitForReadyRead(2 * TIME_OUT);
        delete socket;

        // 其他处理，如：读取启动参数

        activateWindow();
    }
}

void McSingleApplication::initLocalConnection() noexcept 
{
    d->isRunning = false;
    
    QLocalSocket socket;
    socket.connectToServer(d->serverName);
    if (socket.waitForConnected(TIME_OUT)) {
        qWarning() << d->serverName << "already running.";
        d->isRunning = true;
        //! 其他处理，如：将启动参数发送到服务端
        return;
    }

    //! 连接不上服务器，就创建一个
    newLocalServer();
}

void McSingleApplication::newLocalServer() noexcept 
{
    d->localServer = new QLocalServer(this);
    connect(d->localServer, SIGNAL(newConnection()), this, SLOT(slot_newLocalConnection()));
    if (!d->localServer->listen(d->serverName)) {
        qCritical() << "local server listen failure."
                    << "error:" << d->localServer->serverError()
                    << "error str:" << d->localServer->errorString();
        // 此时监听失败，可能是程序崩溃时,残留进程服务导致的,移除之
        if (d->localServer->serverError() == QAbstractSocket::AddressInUseError) {
            QLocalServer::removeServer(d->serverName); //! <-- 重点
            d->localServer->listen(d->serverName); //! 再次监听
        }
    }
}

void McSingleApplication::activateWindow() noexcept 
{
    if(d->mainApp) {
        QMetaObject::invokeMethod(d->mainApp, "activeApp");
    }
}
