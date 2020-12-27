#pragma once

#include <McBoot/McBootGlobal.h>
#include <McIoc/McGlobal.h>
#include <QObject>

MC_FORWARD_DECL_CLASS(Param);
MC_FORWARD_DECL_CLASS(Test);

MC_FORWARD_DECL_PRIVATE_DATA(Config);

class Config : public QObject
{
    Q_OBJECT
    MC_DEFINE_TYPELIST();
    MC_CONFIGURATION
public:
    Q_INVOKABLE explicit Config(QObject *parent = nullptr) noexcept;
    ~Config() override;

    Q_INVOKABLE
    MC_BEAN
    ParamPtr param(const TestPtr &aaa) const noexcept;

private:
    MC_DECL_PRIVATE(Config)
};

MC_DECL_METATYPE(Config)

