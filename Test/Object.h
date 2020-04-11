#pragma once

#include "Interface.h"

#include <QDebug>

class Tmp : public QObject {
    Q_OBJECT
    MC_DECL_INIT(Tmp)
    Q_PROPERTY(QString aaa MEMBER m_aaa)
public:
    Q_INVOKABLE Tmp(){}
    
    QString m_aaa;
};

MC_DECL_METATYPE(Tmp);

class Object : public QObject, public Interface
{
    Q_OBJECT
    MC_DECL_INIT(Object)
    MC_DEFINE_TYPELIST(QObject, MC_DECL_TYPELIST(Interface))
    Q_CLASSINFO(MC_BEANNAME, "obj")     //!< 为本类重新指定beanName，beanName为obj
    Q_PROPERTY(QString text MEMBER m_text)
    Q_PROPERTY(TmpPtr t MEMBER m_t)
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
    TmpPtr m_t;
};

MC_DECL_METATYPE(Object)
