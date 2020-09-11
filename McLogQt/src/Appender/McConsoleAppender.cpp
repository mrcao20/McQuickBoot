#include "McLog/Appender/impl/McConsoleAppender.h"

#include <QFile>

MC_INIT(McConsoleAppender)
MC_REGISTER_BEAN_FACTORY(MC_TYPELIST(McConsoleAppender))
MC_INIT_END

McConsoleAppender::McConsoleAppender() 
{
}

McConsoleAppender::~McConsoleAppender() 
{
}

void McConsoleAppender::finished() noexcept 
{
    McFileDeviceAppender::finished();
    
    QSharedPointer<QFile> file = QSharedPointer<QFile>::create();
    
    if(!file->open(stdout, QIODevice::WriteOnly)) {
        MC_PRINT_ERR("error open stdout for write console!!!\n");
        return;
    }
    
    setDevice(file);
}
