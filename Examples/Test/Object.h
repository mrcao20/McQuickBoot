#pragma once

#include "Interface.h"

#include <QDebug>

class Tmp : public QObject {
    Q_OBJECT
    MC_DECL_INIT(Tmp)
    MC_TYPELIST(QObject)
    MC_COMPONENT("tmp")
    Q_PROPERTY(QString aaa MEMBER m_aaa)
public:
    Q_INVOKABLE Tmp(){}
    
    QString m_aaa;
};

MC_DECL_METATYPE(Tmp);

typedef QHash<int, QList<TmpPtr>> THash;
typedef QHash<QString, THash> TTHash;
class Object : public QObject, public Interface
{
    Q_OBJECT
    MC_DECL_INIT(Object)
    MC_TYPELIST(QObject, Interface)
    MC_COMPONENT("obj")
    Q_PROPERTY(QString text MEMBER m_text)
    Q_PROPERTY(QList<TmpPtr> t MEMBER m_t)
    Q_PROPERTY(THash tt MEMBER m_tt)
public:
    Q_INVOKABLE explicit Object(QObject *parent = nullptr);
    
    void say() noexcept override {
        qDebug() << m_text;
    }
    
signals:
    void signal();
    void signal2();
    
public slots:
    
public:
    QString m_text;
    QList<TmpPtr> m_t;
    THash m_tt;
};

MC_DECL_METATYPE(Object)
