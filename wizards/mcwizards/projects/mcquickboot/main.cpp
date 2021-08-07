%{Cpp:LicenseTemplate}\
%{JS: QtSupport.qtIncludes([], ["QtGui/QGuiApplication", "QtQml/QQmlApplicationEngine"])}
#include <McBoot/McQuickBoot.h>

int main(int argc, char *argv[])
{
@if %{UseVirtualKeyboard}
    qputenv("QT_IM_MODULE", QByteArray("qtvirtualkeyboard"));

@endif
    McQuickBoot::setPreInitFunc([](QCoreApplication *app){
        Q_UNUSED(app)
    });
    McQuickBoot::setAfterInitFunc([](QCoreApplication *app, QQmlApplicationEngine *engine){
        Q_UNUSED(app)
        Q_UNUSED(engine)
    });
@if %{SingleApplication}
    return McQuickBoot::singleRun(argc, argv);
@else
    return McQuickBoot::run(argc, argv);
@endif
}
