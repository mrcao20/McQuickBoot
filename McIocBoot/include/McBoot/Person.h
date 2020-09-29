#ifndef PERSON_H
#define PERSON_H

#include <QObject>
#include "McBootGlobal.h"
#include "Dog.h"

class Person : public QObject
{
    Q_OBJECT
    MC_DECL_INIT(Person)
    MC_COMPONENT
    MC_CONFIGURATION_PROPERTIES("boot.application.person")
    Q_PROPERTY(QString lastName MEMBER m_lastName)
    Q_PROPERTY(int age MEMBER m_age)
    Q_PROPERTY(bool boss MEMBER m_boss)
    using DogMap = QMap<QString, DogPtr>;
    Q_PROPERTY(DogMap map MEMBER m_map)
    Q_PROPERTY(QList<DogPtr> lists MEMBER m_lists)
    Q_PROPERTY(DogPtr dog MEMBER m_dog)
public:
    Q_INVOKABLE Person();
    
public:
    QString m_lastName;
    int m_age;
    bool m_boss;
    DogMap m_map;
    QList<DogPtr> m_lists;
    DogPtr m_dog;
};

MC_DECL_METATYPE(Person)

#endif // PERSON_H
