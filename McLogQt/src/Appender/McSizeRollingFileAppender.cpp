#include "McLog/Appender/impl/McSizeRollingFileAppender.h"

#include <QFile>

MC_DECL_PRIVATE_DATA(McSizeRollingFileAppender)
QString maxFileSize;
qint64 maxFileSizeBytes{-1};        //!< 单位: byte，默认-1表示用不创建新文件
MC_DECL_PRIVATE_DATA_END

MC_INIT(McSizeRollingFileAppender)
MC_REGISTER_BEAN_FACTORY(McSizeRollingFileAppender)
MC_INIT_END

McSizeRollingFileAppender::McSizeRollingFileAppender() 
{
    MC_NEW_PRIVATE_DATA(McSizeRollingFileAppender);
}

McSizeRollingFileAppender::~McSizeRollingFileAppender() 
{
}

QString McSizeRollingFileAppender::maxFileSize() const noexcept 
{
    return d->maxFileSize;
}

void McSizeRollingFileAppender::setMaxFileSize(const QString &val) noexcept 
{
    d->maxFileSize = val.toUpper();
    
    auto temp = d->maxFileSize;
    if(temp.endsWith("GB")) {
        temp.remove("GB");
        bool ok = false;
        auto size = temp.toLongLong(&ok);
        if(ok) {
            setMaxFileSizeBytes(size * 1024 * 1024 * 1024);
        }
    }else if(temp.endsWith("MB")) {
        temp.remove("MB");
        bool ok = false;
        auto size = temp.toLongLong(&ok);
        if(ok) {
            setMaxFileSizeBytes(size * 1024 * 1024);
        }
    }else if(temp.endsWith("KB")) {
        temp.remove("KB");
        bool ok = false;
        auto size = temp.toLongLong(&ok);
        if(ok) {
            setMaxFileSizeBytes(size * 1024);
        }
    }else if(temp.endsWith("B")) {
        temp.remove("B");
        bool ok = false;
        auto size = temp.toLongLong(&ok);
        if(ok) {
            setMaxFileSizeBytes(size);
        }
    }
}

qint64 McSizeRollingFileAppender::maxFileSizeBytes() const noexcept 
{
    return d->maxFileSizeBytes;
}

void McSizeRollingFileAppender::setMaxFileSizeBytes(qint64 val) noexcept 
{
    d->maxFileSizeBytes = val;
}

bool McSizeRollingFileAppender::isNewNextFile() noexcept 
{
    if(d->maxFileSizeBytes == -1) {
        return false;
    }
    if(device().isNull()) {
        return false;
    }
    auto file = device().staticCast<QFile>();
    if(!file->isOpen()) {
        return false;
    }
    return file->size() >= d->maxFileSizeBytes;
}
