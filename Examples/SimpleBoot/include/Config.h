#pragma once

#include <McBoot/McBootGlobal.h>
#include <McIoc/McGlobal.h>
#include <QObject>

MC_FORWARD_DECL_CLASS(Param);
MC_FORWARD_DECL_CLASS(Test);

MC_FORWARD_DECL_PRIVATE_DATA(Config);

using ParamMap = QMap<QString, Param *>;

class Config : public QObject
{
    Q_OBJECT
    MC_TYPELIST()
    MC_CONFIGURATION()
public:
    Q_INVOKABLE explicit Config(QObject *parent = nullptr) noexcept;
    ~Config() override;

    Q_INVOKABLE
    MC_BEAN
    ParamPtr param(QThread *threadTest) const noexcept;

    Q_INVOKABLE
    MC_BEAN
    ParamMap paramMap() const noexcept;

    Q_INVOKABLE
    MC_BEAN_FINISHED
    void finished();

private slots:
    void slot_slt2();

private:
    MC_DECL_PRIVATE(Config)
};

MC_DECL_METATYPE(Config)

