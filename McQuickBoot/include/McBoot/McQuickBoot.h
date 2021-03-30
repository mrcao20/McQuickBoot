#pragma once

#include "McAbstractQuickBoot.h"

#include <functional>

#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QUrl>

#include "Application/McSingleApplication.h"
#include "Application/McSingleCoreApplication.h"

QT_BEGIN_NAMESPACE
class QQuickView;
class QQuickWidget;
QT_END_NAMESPACE

MC_FORWARD_DECL_CLASS(IMcApplicationContext);

MC_FORWARD_DECL_PRIVATE_DATA(McQuickBoot);

using std::function;

class MCQUICKBOOT_EXPORT McQuickBoot : public McAbstractQuickBoot
{
    Q_OBJECT
    MC_DECL_INIT(McQuickBoot)
public:
    explicit McQuickBoot(QObject *parent = nullptr);
    ~McQuickBoot() override;

    static void init(QCoreApplication *app, QQmlApplicationEngine *engine) noexcept;
    static QQmlEngine *engine() noexcept;
    static QQuickView *createQuickView(const QString &source, QWindow *parent = nullptr) noexcept;
    static QQuickWidget *createQuickWidget(const QString &source,
                                           QWidget *parent = nullptr) noexcept;

    static QSharedPointer<McQuickBoot> instance() noexcept;

    static void setPreInitFunc(const function<void(QCoreApplication *)> &func) noexcept;
    static void setAfterInitFunc(
        const function<void(QCoreApplication *, QQmlApplicationEngine *)> &func) noexcept;

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

    //! 如果重新或新加载了某些组件，则调用此函数
    void refresh() noexcept;
    IMcApplicationContextPtr getApplicationContext() const noexcept override;

protected:
    void initContainer() const noexcept override;

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
