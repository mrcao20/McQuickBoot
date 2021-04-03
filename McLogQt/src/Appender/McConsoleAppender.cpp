#include "McLog/Appender/impl/McConsoleAppender.h"

#include <QFile>
#include <QTextCodec>

MC_INIT(McConsoleAppender)
MC_REGISTER_BEAN_FACTORY(McConsoleAppender)
MC_INIT_END

McConsoleAppender::McConsoleAppender() 
{
}

McConsoleAppender::~McConsoleAppender() 
{
}

void McConsoleAppender::doFinished() noexcept
{
    McFileDeviceAppender::doFinished();

    QSharedPointer<QFile> file = QSharedPointer<QFile>::create();

    if (!file->open(stderr, QIODevice::WriteOnly)) {
        MC_PRINT_ERR("error open stdout for write console!!!\n");
        return;
    }

    setDevice(file);
    setCodec(QTextCodec::codecForLocale());
}
