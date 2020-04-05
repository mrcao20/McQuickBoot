#include "include/McIocBoot.h"

#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QJSValue>
#include <QDebug>

#include <ApplicationContext/impl/McAnnotationApplicationContext.h>
#include <BeanDefinition/IMcBeanDefinition.h>

#include "include/Controller/impl/McControllerContainer.h"
#include "include/Model/McModelContainer.h"
#include "include/Socket/impl/McQmlSocketContainer.h"
#include "include/Requestor/McQmlRequestor.h"

MC_DECL_PRIVATE_DATA(McIocBoot)
McAnnotationApplicationContextPtr context;
MC_DECL_PRIVATE_DATA_END

McIocBoot::McIocBoot(QObject *parent)
    : QObject(parent)
{
    MC_NEW_PRIVATE_DATA(McIocBoot)
}

McIocBoot::~McIocBoot() {
}

int McIocBoot::run(int argc, char *argv[]
                   , const function<void(QCoreApplication *app, QJSEngine *)> &func) noexcept {
    
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QGuiApplication app(argc, argv);
    
    McIocBootPtr boot = McIocBootPtr::create();
    boot->initBoot();
    
    McControllerContainerPtr controllerContainer = McControllerContainerPtr::create();
    controllerContainer->init(boot);
    
    McModelContainerPtr modelContainer = McModelContainerPtr::create();
    modelContainer->init(boot);
    
    McQmlSocketContainerPtr socketContainer = McQmlSocketContainerPtr::create();
    socketContainer->init(boot);
    
    QQmlApplicationEngine engine;
    
    McQmlRequestor *requestor = new McQmlRequestor(&engine); //!< 不需要设置父对象
    requestor->setControllerContainer(controllerContainer);
    requestor->setSocketContainer(socketContainer);
    
    //! engine的newQObject函数会将其参数所有权转移到其返回的QJSValue中
    QJSValue jsObj = engine.newQObject(requestor);
    engine.globalObject().setProperty("$", jsObj);
    engine.importModule(":/Requestor.js");
    
    if(func) {
        func(&app, &engine);
    }
    
    const QUrl url(QStringLiteral("qrc:/main.qml"));
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);
    engine.load(url);

    return app.exec();
}

void McIocBoot::initBoot() noexcept {
    if (d->context) {
		qInfo() << "The container has been initialized";
		return;
	}
    d->context = McAnnotationApplicationContextPtr::create();
    d->context->refresh();  //!< 预加载bean
}

QSharedPointer<IMcApplicationContext> McIocBoot::getApplicationContext() const noexcept {
    return d->context;
}

QList<QString> McIocBoot::getComponents(const QString &componentType) noexcept {
    auto context = getApplicationContext();
	if (!context) {
		qCritical() << "Please call initContainer to initialize container first";
		return QList<QString>();
	}
	QList<QString> components;
    QHash<QString, IMcBeanDefinitionPtr> beanDefinitions = context->getBeanDefinitions();
	for (auto itr = beanDefinitions.cbegin(); itr != beanDefinitions.cend(); ++itr) {
		auto beanDefinition = itr.value();
		if (!isComponentType(beanDefinition->getBeanMetaObject(), componentType))
			continue;
		components.append(itr.key());
	}
	return components;
}

bool McIocBoot::isComponentType(const QMetaObject *metaObj, const QString &type) noexcept {
    if(!metaObj) {
        return false;
    }
	int classInfoCount = metaObj->classInfoCount();
	for (int i = 0; i < classInfoCount; ++i) {
		auto classInfo = metaObj->classInfo(i);
		if (qstrcmp(classInfo.name(), "Component") != 0)
			continue;
		return classInfo.value() == type;
	}
	return false;
}
