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
    McSingleApplication(int& argc, char** argv) noexcept;
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
