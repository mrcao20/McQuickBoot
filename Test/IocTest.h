#pragma once

#include "McIoc/McGlobal.h"

#include <QDebug>

#include "Interface.h"

class IocTest : public QObject
{
    Q_OBJECT
    MC_DECL_INIT(IocTest)
    MC_DEFINE_TYPELIST(QObject)
    Q_CLASSINFO(MC_BEANNAME, "test")
    Q_CLASSINFO(MC_SINGLETON, "false")
    Q_CLASSINFO("interface", "obj")
    Q_PROPERTY(InterfacePtr interface MEMBER m_interface)
    Q_PROPERTY(QList<QString> str MEMBER m_str)
    Q_PROPERTY(QVector<InterfacePtr> interfaces MEMBER m_interfaces)
    using QStringMap = QMap<QString, QString>;
    Q_PROPERTY(QStringMap strMap MEMBER m_strMap)
    using IHash = QHash<QString, InterfacePtr>;
    Q_PROPERTY(IHash iMap MEMBER m_iMap)
public:
    Q_INVOKABLE explicit IocTest(QObject *parent = nullptr);
    
signals:
    
public slots:
    void slot() {
        qDebug() << "slot";
    }
    void slot2() {
        qDebug() << "slot2";
    }
    
public:
    InterfacePtr m_interface;
    QList<QString> m_str;
    QVector<InterfacePtr> m_interfaces;
    QMap<QString, QString> m_strMap;
    QHash<QString, InterfacePtr> m_iMap;
};

MC_DECL_METATYPE(IocTest)

