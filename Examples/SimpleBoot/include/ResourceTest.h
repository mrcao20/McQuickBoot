#pragma once

#include <McIoc/McGlobal.h>

class III
{
public:
    virtual ~III() = default;
};
MC_DECL_METATYPE(III)

class ResourceTest : public QObject, public III
{
    Q_OBJECT
    MC_COMPONENT()
    MC_INTERFACES(III)
public:
};

MC_DECL_METATYPE(ResourceTest)

class ResourceTest2 : public QObject, public III
{
    Q_OBJECT
    MC_COMPONENT()
    MC_INTERFACES(III)
public:
};

MC_DECL_METATYPE(ResourceTest2)
