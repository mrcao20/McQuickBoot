#include "McBoot/Controller/InnerController/McApplicationController.h"

#include <QCoreApplication>
#include <QDebug>

MC_DECL_PRIVATE_DATA(McApplicationController)
MC_DECL_PRIVATE_DATA_END

MC_INIT(McApplicationController)
MC_REGISTER_BEAN_FACTORY(MC_TYPELIST(McApplicationController))
MC_INIT_END

McApplicationController::McApplicationController() noexcept
{
    MC_NEW_PRIVATE_DATA(McApplicationController);
}

McApplicationController::~McApplicationController() noexcept {
}

QString McApplicationController::dirPath() const noexcept
{
    return qApp->applicationDirPath();
}

QString McApplicationController::filePath() const noexcept
{
    return qApp->applicationFilePath();
}
