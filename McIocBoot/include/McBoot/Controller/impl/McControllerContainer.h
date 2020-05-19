#pragma once

#include "../IMcControllerContainer.h"

#include <QMap>

QT_BEGIN_NAMESPACE
class QMetaMethod;
QT_END_NAMESPACE

MC_FORWARD_DECL_CLASS(McIocBoot)

MC_FORWARD_DECL_PRIVATE_DATA(McControllerContainer);

class MCIOCBOOT_EXPORT McControllerContainer 
        : public QObject
        , public IMcControllerContainer {
    
	Q_OBJECT
public:
	explicit McControllerContainer(QObject *parent = nullptr);
	~McControllerContainer() override;

    void init(McIocBootConstPtrRef boot) noexcept;
    
    QVariant invoke(const QString &uri, const QVariant &body) noexcept override;

private:
    QVariant invoke(const QString &uri) noexcept;
	QVariant invoke(const QString &uri, const QJsonObject &data) noexcept;
    
    bool splitBeanAndFunc(const QString &uri
                          , QObjectPtr &bean
                          , QString &func
                          , QVariant &errRet) noexcept;
    
	QVariant invokeForUri(QObjectConstPtrRef bean
                          , const QString &func
                          ,  const QMap<QString, QVariant> &args) noexcept;
    
	QMap<QString, QVariant> splitParam(const QString &param) noexcept;
    
	bool isMethodMatching(const QMetaMethod &m, const QList<QString> &argNames) noexcept;
    
	void removeDuplication(QList<QString> &argNames
                           , QList<QByteArray> &paramNames
                           , QList<QByteArray> &paramTypes) noexcept;
    
	QVariant invokeForArgs(QObjectConstPtrRef bean
                           , const QMetaMethod &method
                           , const QMap<QString, QVariant> &args) noexcept;
    QVariantList makeValues(const QMetaMethod &method
                            , const QMap<QString, QVariant> &args
                            , int maxParamSize
                            , QVariant *errMsg = nullptr
                            , bool *ok = nullptr) noexcept;
    
    QVariant makeObjectValue(const QByteArray &typeName, const QVariant &arg) noexcept;
    bool isSharedPointerObject(const QByteArray &typeName, QByteArray &objTypeName) noexcept;
    
    QVariant ok(const QVariant &val) const noexcept;
    QVariant fail(const QString &val) const noexcept;

private:
	MC_DECL_PRIVATE(McControllerContainer)
};

MC_DECL_POINTER(McControllerContainer)