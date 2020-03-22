#pragma once

#include "Interface.h"

#include <QDebug>

class Object : public QObject, public Interface
{
    Q_OBJECT
    MC_DECL_INIT(Object)
    MC_DEFINE_TYPELIST(QObject, MC_DECL_TYPELIST(Interface))
    Q_CLASSINFO(MC_BEANNAME, "obj")
    Q_PROPERTY(QString text MEMBER m_text)
public:
    Q_INVOKABLE explicit Object(QObject *parent = nullptr);
    
    void say() noexcept override {
        qDebug() << m_text;
    }
    
signals:
    void signal();
    
public slots:
    
private:
    QString m_text;
};

MC_DECL_POINTER(Object)
MC_DECL_METATYPE(Object)
