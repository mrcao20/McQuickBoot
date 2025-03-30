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

#ifdef MC_ENABLE_QML

# include <QGuiApplication>
# include <QQmlApplicationEngine>

# include "../Invoker/McCppInvoker.h"
# include "McAbstractQuickBoot.h"

MC_FORWARD_DECL_PRIVATE_DATA(McQuickBoot)

class MC_QUICKBOOT_EXPORT McQuickBoot : public McAbstractQuickBoot
{
    Q_OBJECT
    MC_DECL_SUPER(McAbstractQuickBoot)
public:
    explicit McQuickBoot(QObject *parent = nullptr) noexcept;
    ~McQuickBoot() override;

    static void init(QCoreApplication *app, QQmlApplicationEngine *engine) noexcept;
    static QSharedPointer<McQuickBoot> instance() noexcept;

    static void setPreInitFunc(const std::function<void(QCoreApplication *)> &func) noexcept;
    static void setAfterInitFunc(const std::function<void(QCoreApplication *, QQmlApplicationEngine *)> &func) noexcept;

    template<typename App = QGuiApplication, typename Extra = QQmlApplicationEngine>
    static int run(int argc, char *argv[], const QString &path = QStringLiteral("qrc:/main.qml")) noexcept;

    McCppInvoker &invoker() const noexcept;

protected:
    void loadInvoker(IMcApplicationContext *appCtx, QThread *workThread) noexcept override;
    void flushInvoker(IMcApplicationContext *appCtx) noexcept override;

private:
    MC_DECL_PRIVATE(McQuickBoot)
};

MC_DECL_POINTER(McQuickBoot)

//! 此宏所对应的对象将在Application析构时销毁，所以一旦Application开始析构，就再也不要调用此宏
# ifdef $
#  undef $
# endif
# define $ (McQuickBoot::instance()->invoker())

template<typename App, typename Extra>
int McQuickBoot::run(int argc, char *argv[], const QString &path) noexcept
{
    App app(argc, argv);

    if constexpr (std::is_same_v<Extra, QQmlApplicationEngine>) {
        Extra engine;

        McQuickBoot::init(&app, &engine);

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
    } else {
        McQuickBoot::init(&app, nullptr);

        if constexpr (std::is_void_v<Extra>) {
            return app.exec();
        } else {
            Extra w;
            w.show();
            return app.exec();
        }
    }
}

#else

# include "McCppQuickBoot.h"

using McQuickBoot = McCppQuickBoot;

#endif
