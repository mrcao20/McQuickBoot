#pragma once

#include "McIoc/McGlobal.h"

#include <McIoc/Thread/impl/McDefaultDeleteThreadWhenQuit.h>
#include <QDebug>

#include "Interface.h"
#include "Object.h"

class IocTest : public QObject, public McDefaultDeleteThreadWhenQuit
{
    Q_OBJECT
    MC_DECL_INIT(IocTest)
    MC_TYPELIST(QObject, McDefaultDeleteThreadWhenQuit)
    MC_COMPONENT
    MC_BEANNAME("test")
    MC_SINGLETON(false)
    MC_AUTOWIRED("interface = obj")
    Q_PROPERTY(InterfacePtr interface MEMBER m_interface)
    Q_PROPERTY(QObjectPtr innerBean MEMBER m_innerBean)
    Q_PROPERTY(QList<QString> str MEMBER m_str)
    Q_PROPERTY(QVector<InterfacePtr> interfaces MEMBER m_interfaces)
    using QStringMap = QMap<QString, QString>;
    Q_PROPERTY(QStringMap strMap MEMBER m_strMap)
    using IHash = QHash<QString, InterfacePtr>;
    Q_PROPERTY(IHash iMap MEMBER m_iMap)
    Q_PROPERTY(EEE eee MEMBER m_eee)
#if QT_VERSION >= QT_VERSION_CHECK(5, 5, 0)
    Q_PROPERTY(Qt::AlignmentFlag align MEMBER m_align)
#endif
public:
    enum EEE {
        A = 1,
        B
    };
#if QT_VERSION >= QT_VERSION_CHECK(5, 5, 0)
    Q_ENUM(EEE)
#else
    Q_ENUMS(EEE)
#endif
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
    QObjectPtr m_innerBean;
    QList<QString> m_str;
    QVector<InterfacePtr> m_interfaces;
    QMap<QString, QString> m_strMap;
    QHash<QString, InterfacePtr> m_iMap;
    EEE m_eee;
    Qt::AlignmentFlag m_align;
};

MC_DECL_METATYPE(IocTest)

