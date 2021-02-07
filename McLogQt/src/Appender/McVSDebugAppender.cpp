#include "McLog/Appender/impl/McVSDebugAppender.h"

#include "McLog/Device/McVSDebugDevice.h"

MC_INIT(McVSDebugAppender)
MC_REGISTER_BEAN_FACTORY(McVSDebugAppender)
MC_INIT_END

MC_DECL_PRIVATE_DATA(McVSDebugAppender)
MC_DECL_PRIVATE_DATA_END

McVSDebugAppender::McVSDebugAppender() noexcept
{
    MC_NEW_PRIVATE_DATA(McVSDebugAppender);
}

McVSDebugAppender::~McVSDebugAppender()
{
}

void McVSDebugAppender::finished() noexcept
{
    McAbstractFormatAppender::finished();

    auto dev = McVSDebugDevicePtr::create(this);
    if (!dev->open(QIODevice::WriteOnly)) {
        MC_PRINT_ERR("error open vs debuger for write!!!\n");
        return;
    }

    setDevice(dev);
}

void McVSDebugAppender::writeBefore() noexcept
{
}

void McVSDebugAppender::writeAfter() noexcept
{
}
