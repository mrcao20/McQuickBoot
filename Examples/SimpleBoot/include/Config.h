#pragma once

#include <McBoot/McBootGlobal.h>
#include <McIoc/McGlobal.h>
#include <QObject>

#include <McBoot/Configuration/IMcQuickBootConfigurer.h>

MC_FORWARD_DECL_CLASS(Param);
MC_FORWARD_DECL_CLASS(Test);

MC_FORWARD_DECL_PRIVATE_DATA(Config);

using ParamMap = QMap<QString, Param *>;

class Config : public QObject, public IMcQuickBootConfigurer
{
    Q_OBJECT
    MC_INTERFACES(IMcQuickBootConfigurer)
    MC_CONFIGURATION()
public:
    Q_INVOKABLE explicit Config(QObject *parent = nullptr) noexcept;
    ~Config() override;

    void addResponseHandler(McResponseHandlerRegistry &registry) noexcept override;

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

