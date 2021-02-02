#pragma once

#include <McBoot/McBootGlobal.h>
#include <McIoc/McGlobal.h>
#include <QDebug>
#include <QObject>
#include <QThread>

MC_FORWARD_DECL_PRIVATE_DATA(Config2);

class CThread : public QThread
{
    Q_OBJECT
public:
    CThread() { qDebug() << "CThread"; }
    ~CThread() { qDebug() << "~CThread"; }
};

class Config2 : public QObject
{
    Q_OBJECT
    MC_TYPELIST()
    MC_CONFIGURATION(1)
public:
    Q_INVOKABLE explicit Config2(QObject *parent = nullptr) noexcept;
    ~Config2() override;

    Q_INVOKABLE
    MC_BEAN
    QThread *threadTest() const noexcept;

    MC_DECL_QUICKBOOT_WORK_THREAD

private:
    MC_DECL_PRIVATE(Config2)
};

MC_DECL_METATYPE(Config2)
