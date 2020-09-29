#ifndef DOG_H
#define DOG_H

#include <QObject>
#include "McBootGlobal.h"

class Dog : public QObject
{
    Q_OBJECT
    MC_DECL_INIT(Dog)
    Q_PROPERTY(QString name MEMBER m_name)
    Q_PROPERTY(int age MEMBER m_age)
public:
    Q_INVOKABLE explicit Dog(QObject *parent = 0);
    
public:
    QString m_name;
    int m_age;
};

MC_DECL_METATYPE(Dog)

#endif // DOG_H
