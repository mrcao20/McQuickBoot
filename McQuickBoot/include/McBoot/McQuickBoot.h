#pragma once

#include "IMcQuickBoot.h"

#include <functional>

#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QUrl>

#include "Application/McSingleApplication.h"
#include "Application/McSingleCoreApplication.h"
#include "McBoot/Requestor/McQmlRequestor.h"

//! 此宏所对应的对象将在Application析构时销毁，所以一旦Application开始析构，就再也不要调用此宏
#define $ McQuickBoot::requestor()

QT_BEGIN_NAMESPACE
class QQuickView;
QT_END_NAMESPACE

MC_FORWARD_DECL_CLASS(IMcApplicationContext);

MC_FORWARD_DECL_PRIVATE_DATA(McQuickBoot);

using std::function;

class MCQUICKBOOT_EXPORT McQuickBoot : public QObject, public IMcQuickBoot
{
    Q_OBJECT
    MC_DECL_INIT(McQuickBoot)
public:
    explicit McQuickBoot(QObject *parent = nullptr);
    ~McQuickBoot() override;
    
    static void init(QCoreApplication *app, QQmlApplicationEngine *engine) noexcept;
    static QQmlEngine *engine() noexcept;
    static QQuickView *createQuickView(const QString &source, QWindow *parent = nullptr) noexcept;
    
    static void setPreInitFunc(const function<void(QCoreApplication *)> &func) noexcept;
    static void setAfterInitFunc(const function<void(QCoreApplication *, QQmlApplicationEngine *)> &func) noexcept;
    
    static McQmlRequestorPtr requestor() noexcept;
    
    template<typename T = QGuiApplication>
    static int run(int argc, char *argv[], const QString &path = "qrc:/main.qml") noexcept;
    
    /*!
     * \brief singleRun
     * 
     * 单例运行应用程序，url指定的qml文件的objectName必须为mainApp
     * \param argc
     * \param argv
     * \param url
     * \return 
     */
    template<typename T = McSingleApplication>
    static int singleRun(int argc, char *argv[], const QString &path = "qrc:/main.qml") noexcept;
 
    void initBoot(QQmlEngine *engine) noexcept;
    
    IMcApplicationContextPtr getApplicationContext() const noexcept override;
    
    //! 获取所有被Component标记的bean
    QList<QString> getAllComponent() noexcept;
    //! 获取所有组建类型为componentType的bean的名称
    QList<QString> getComponents(const QString &componentType) noexcept;
    
private:
    MC_DECL_PRIVATE(McQuickBoot)
};

MC_DECL_POINTER(McQuickBoot);

template<typename T>
int McQuickBoot::run(int argc, char *argv[], const QString &path) noexcept 
{
#if QT_VERSION >= QT_VERSION_CHECK(5, 6, 0)
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#endif

    T app(argc, argv);
    
    QQmlApplicationEngine engine;
    
    McQuickBoot::init(&app, &engine);
    
    QString dstPath = Mc::toAbsolutePath(path);
    QUrl url(dstPath);
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated
                     , &app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);
    engine.load(url);

    return app.exec();
}

template<typename T>
int McQuickBoot::singleRun(int argc, char *argv[], const QString &path) noexcept 
{
#if QT_VERSION >= QT_VERSION_CHECK(5, 6, 0)
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#endif

    T app(argc, argv);
    if(app.isRunning())
        return 0;
    
    QQmlApplicationEngine engine;
    
    McQuickBoot::init(&app, &engine);
    
    QString dstPath = Mc::toAbsolutePath(path);
    QUrl url(dstPath);
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated
                     , &app, [url, &app](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
        if (obj && obj->objectName() == "mainApp") {
            app.setAppObj(obj);
        }
    }, Qt::QueuedConnection);
    engine.load(url);

    return app.exec();
}
