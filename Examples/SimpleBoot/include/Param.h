#pragma once

#include <McIoc/McGlobal.h>
#include <QDebug>
#include <QObject>

#include <McBoot/McBootGlobal.h>

MC_FORWARD_DECL_PRIVATE_DATA(Param);

class Param : public QObject
{
    Q_OBJECT
    MC_TYPELIST();
    MC_JSON_SERIALIZATION()
public:
    Q_INVOKABLE explicit Param(QObject *parent = nullptr) noexcept;
    ~Param() override;

    MC_POCO_PROPERTY(int, aaa);

public slots:
    void slot_slt();
    void callbackTest3(int a) { qDebug() << "Param callback test3:" << a; }

private:
    MC_DECL_PRIVATE(Param)
};

MC_DECL_METATYPE(Param)

