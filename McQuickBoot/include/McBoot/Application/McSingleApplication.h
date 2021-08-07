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
#pragma once

#include "../McBootGlobal.h"

#include <QGuiApplication>

#include <McIoc/BeanFactory/McBeanGlobal.h>

QT_BEGIN_NAMESPACE
class QLocalServer;
QT_END_NAMESPACE

MC_FORWARD_DECL_PRIVATE_DATA(McSingleApplication);

/*!
 * \brief The McSingleApplication class
 * 
 * 单例gui应用程序，使用QLocalServer实现，程序运行之初需要设置一个关联
 * 整个应用程序的单例object，如果程序正在运行则会调用该对象中的activeApp函数
 */
class MCQUICKBOOT_EXPORT McSingleApplication : public QGuiApplication 
{
    Q_OBJECT
public:
    McSingleApplication(int &argc, char **argv) noexcept;
    ~McSingleApplication() noexcept;
    
    /*!
     * \brief isRunning
     * 
     * 是否已经有实例正在运行
     * \return 
     */
    bool isRunning() noexcept;
    
    /*!
     * \brief setAppObj
     * 
     * 设置关联单例应用的object，如果程序正在被运行，则会调用该对象中的activeApp函数
     * 故设置进来的对象需要包含一个在QT元对象系统中的activeApp函数
     * \param obj
     */
    void setAppObj(QObject *obj) noexcept;
    
private slots:
    /*!
     * \brief slot_newLocalConnection
     * 
     * 有新连接时触发
     */
    void slot_newLocalConnection() noexcept;
    
private:
    /*!
     * \brief initLocalConnection
     * 
     * 初始化本地连接
     */
    void initLocalConnection() noexcept;
    /*!
     * \brief newLocalServer
     * 
     * 创建服务端
     */
    void newLocalServer() noexcept;
    /*!
     * \brief activateWindow
     * 
     * 激活
     */
    void activateWindow() noexcept;

private:
    MC_DECL_PRIVATE(McSingleApplication)
};
