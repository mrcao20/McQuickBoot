#pragma once

#include "McBootGlobal.h"

#include <functional>

#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QUrl>

#include "Application/McSingleApplication.h"
#include "Application/McSingleCoreApplication.h"

QT_BEGIN_NAMESPACE
class QQuickView;
QT_END_NAMESPACE

MC_FORWARD_DECL_CLASS(IMcApplicationContext);

MC_FORWARD_DECL_PRIVATE_DATA(McIocBoot);

using std::function;

class MCIOCBOOT_EXPORT McIocBoot : public QObject 
{
    Q_OBJECT
public:
    explicit McIocBoot(QObject *parent = nullptr);
    ~McIocBoot() override;
    
    static void init(QQmlApplicationEngine *engine) noexcept;
    static QQmlEngine *engine() noexcept;
    static QQuickView *createQuickView(const QString &source, QWindow *parent = nullptr) noexcept;
    
    template<typename T = QGuiApplication>
    static int run(int argc, char *argv[], const QString &path = "qrc:/main.qml"
            , const function<void(T *app, QQmlApplicationEngine *)> &func = nullptr
            , const function<void(T *app)> &prefunc = nullptr) noexcept;
    
    /*!
     * \brief singleRun
     * 
     * 单例运行应用程序，url指定的qml文件的objectName必须为mainApp
     * \param argc
     * \param argv
     * \param url
     * \param func
     * \return 
     */
    template<typename T = McSingleApplication>
    static int singleRun(int argc, char *argv[], const QString &path = "qrc:/main.qml"
            , const function<void(T *app, QQmlApplicationEngine *)> &func = nullptr
            , const function<void(T *app)> &prefunc = nullptr) noexcept;
 
    void initBoot(QQmlEngine *engine) noexcept;
    
    IMcApplicationContextPtr getApplicationContext() const noexcept;
    
    //! 获取所有被Component标记的bean
    QList<QString> getAllComponent() noexcept;
    //! 获取所有组建类型为componentType的bean的名称
    QList<QString> getComponents(const QString &componentType) noexcept;
    
private:
    MC_DECL_PRIVATE(McIocBoot)
};

MC_DECL_POINTER(McIocBoot);

template<typename T>
int McIocBoot::run(int argc, char *argv[], const QString &path
                   , const function<void(T *app, QQmlApplicationEngine *)> &func
                   , const function<void(T *app)> &prefunc) noexcept 
{
#if QT_VERSION >= QT_VERSION_CHECK(5, 6, 0)
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#endif

    T app(argc, argv);
    
    if(prefunc) {
        prefunc(&app);
    }
    
    QQmlApplicationEngine engine;
    
    McIocBoot::init(&engine);
    
    if(func) {
        func(&app, &engine);
    }
    
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
int McIocBoot::singleRun(int argc, char *argv[], const QString &path
                         , const function<void(T *app, QQmlApplicationEngine *)> &func
                         , const function<void(T *app)> &prefunc) noexcept 
{
#if QT_VERSION >= QT_VERSION_CHECK(5, 6, 0)
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#endif

    T app(argc, argv);
    if(app.isRunning())
        return 0;
    
    if(prefunc) {
        prefunc(&app);
    }
    
    QQmlApplicationEngine engine;
    
    McIocBoot::init(&engine);
    
    if(func) {
        func(&app, &engine);
    }
    
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
