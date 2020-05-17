#pragma once

#include "McBootGlobal.h"

#include <functional>

#include <QUrl>

QT_BEGIN_NAMESPACE
class QCoreApplication;
class QJSEngine;
QT_END_NAMESPACE

MC_FORWARD_DECL_CLASS(IMcApplicationContext);

MC_FORWARD_DECL_PRIVATE_DATA(McIocBoot);

using std::function;

class MCIOCBOOT_EXPORT McIocBoot : public QObject {
    Q_OBJECT
public:
    explicit McIocBoot(QObject *parent = nullptr);
    ~McIocBoot() override;
    
    static int run(int argc, char *argv[], const QUrl &url = QUrl(QStringLiteral("qrc:/main.qml"))
                   , const function<void(QCoreApplication *app, QJSEngine *)> &func = nullptr) noexcept;
    
    void initBoot() noexcept;
    
    IMcApplicationContextPtr getApplicationContext() const noexcept;
    
    //! 获取所有组建类型为componentType的bean的名称
    QList<QString> getComponents(const QString &componentType) noexcept;
    //! 传入的元对象的组件类型是否为type
    bool isComponentType(const QMetaObject *metaObj, const QString &type) noexcept;
    
private:
    MC_DECL_PRIVATE(McIocBoot)
};

MC_DECL_POINTER(McIocBoot)
