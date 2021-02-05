#include "McLog/Appender/impl/McFileDeviceAppender.h"

#include <QFileDevice>

MC_INIT(McFileDeviceAppender)
MC_REGISTER_BEAN_FACTORY(McFileDeviceAppender)
MC_INIT_END

McFileDeviceAppender::McFileDeviceAppender() 
{
}

McFileDeviceAppender::~McFileDeviceAppender() 
{
}

void McFileDeviceAppender::writeBefore() noexcept
{
    
}

void McFileDeviceAppender::writeAfter() noexcept 
{
    auto fileDevice = device().staticCast<QFileDevice>();
    
    if(fileDevice.isNull()) {
        return;
    }

    tryNextFile();
}

void McFileDeviceAppender::tryNextFile() noexcept 
{
}
