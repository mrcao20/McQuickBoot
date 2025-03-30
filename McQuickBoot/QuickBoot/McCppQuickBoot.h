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

#include "../Invoker/McCppInvoker.h"
#include "McAbstractQuickBoot.h"

class QCoreApplication;

MC_FORWARD_DECL_PRIVATE_DATA(McCppQuickBoot)

class MC_QUICKBOOT_EXPORT McCppQuickBoot : public McAbstractQuickBoot
{
    Q_OBJECT
    MC_DECL_SUPER(McAbstractQuickBoot)
public:
    explicit McCppQuickBoot(QObject *parent = nullptr) noexcept;
    ~McCppQuickBoot() override;

    static void init(QCoreApplication *app) noexcept;
    static QSharedPointer<McCppQuickBoot> instance() noexcept;

    static void setPreInitFunc(const std::function<void(QCoreApplication *)> &func) noexcept;
    static void setAfterInitFunc(const std::function<void(QCoreApplication *)> &func) noexcept;

    template<typename App = QCoreApplication, typename Widget = void>
    static int run(int argc, char *argv[]) noexcept;

    McCppInvoker &invoker() const noexcept;

protected:
    void loadInvoker(IMcApplicationContext *appCtx, QThread *workThread) noexcept override;
    void flushInvoker(IMcApplicationContext *appCtx) noexcept override;

private:
    MC_DECL_PRIVATE(McCppQuickBoot)
};

MC_DECL_POINTER(McCppQuickBoot)

//! 此宏所对应的对象将在Application析构时销毁，所以一旦Application开始析构，就再也不要调用此宏
#ifdef $
# undef $
#endif
#define $ (McCppQuickBoot::instance()->invoker())

template<typename App, typename Widget>
int McCppQuickBoot::run(int argc, char *argv[]) noexcept
{
    App app(argc, argv);

    McCppQuickBoot::init(&app);

    if constexpr (std::is_void_v<Widget>) {
        return app.exec();
    } else {
        Widget w;
        w.show();
        return app.exec();
    }
}
