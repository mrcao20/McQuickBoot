#pragma once

#include <QObject>
#include <McIoc/McGlobal.h>

MC_FORWARD_DECL_PRIVATE_DATA(Param);

class Param : public QObject
{
    Q_OBJECT
    MC_DEFINE_TYPELIST();
public:
    Q_INVOKABLE explicit Param(QObject *parent = nullptr) noexcept;
    ~Param() override;

    MC_POCO_PROPERTY(int, aaa);

private:
    MC_DECL_PRIVATE(Param)
};

MC_DECL_METATYPE(Param)

