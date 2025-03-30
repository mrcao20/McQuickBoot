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

#include <QGuiApplication>
#include <QQmlApplicationEngine>

#include "McAbstractQuickBoot.h"

MC_FORWARD_DECL_PRIVATE_DATA(McQmlQuickBoot)

class MC_QUICKBOOT_EXPORT McQmlQuickBoot : public McAbstractQuickBoot
{
    Q_OBJECT
    MC_DECL_SUPER(McAbstractQuickBoot)
public:
    explicit McQmlQuickBoot(QObject *parent = nullptr) noexcept;
    ~McQmlQuickBoot() override;

    static void init(QCoreApplication *app, QQmlApplicationEngine *engine) noexcept;
    static QSharedPointer<McQmlQuickBoot> instance() noexcept;

    static void setPreInitFunc(const std::function<void(QCoreApplication *)> &func) noexcept;
    static void setAfterInitFunc(const std::function<void(QCoreApplication *, QQmlApplicationEngine *)> &func) noexcept;

    template<typename App = QGuiApplication, typename Engine = QQmlApplicationEngine>
    static int run(int argc, char *argv[], const QString &path = QStringLiteral("qrc:/main.qml")) noexcept;

protected:
    void loadInvoker(IMcApplicationContext *appCtx, QThread *workThread) noexcept override;
    void flushInvoker(IMcApplicationContext *appCtx) noexcept override;

private:
    MC_DECL_PRIVATE(McQmlQuickBoot)
};

MC_DECL_POINTER(McQmlQuickBoot)

template<typename App, typename Engine>
int McQmlQuickBoot::run(int argc, char *argv[], const QString &path) noexcept
{
    App app(argc, argv);

    QQmlApplicationEngine engine;

    McQmlQuickBoot::init(&app, &engine);

    QString dstPath = Mc::toAbsolutePath(path);
    QUrl url(dstPath);
    QObject::connect(
        &engine, &QQmlApplicationEngine::objectCreated, &app,
        [url](QObject *obj, const QUrl &objUrl) {
            if (!obj && url == objUrl)
                QCoreApplication::exit(-1);
        },
        Qt::QueuedConnection);
    engine.load(url);

    return app.exec();
}
