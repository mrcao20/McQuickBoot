#pragma once

#include "McRollingFileAppender.h"

MC_FORWARD_DECL_PRIVATE_DATA(McSizeRollingFileAppender);

class MCLOGQT_EXPORT McSizeRollingFileAppender : public McRollingFileAppender 
{
    Q_OBJECT
    MC_DECL_INIT(McSizeRollingFileAppender)
    MC_TYPELIST(McRollingFileAppender)
    Q_PROPERTY(QString maxFileSize READ maxFileSize WRITE setMaxFileSize)
    Q_PROPERTY(qint64 maxFileSizeBytes READ maxFileSizeBytes WRITE setMaxFileSizeBytes)
public:
    Q_INVOKABLE McSizeRollingFileAppender();
    ~McSizeRollingFileAppender() override;
    
    QString maxFileSize() const noexcept;
    void setMaxFileSize(const QString &val) noexcept;
    
    qint64 maxFileSizeBytes() const noexcept;
    void setMaxFileSizeBytes(qint64 val) noexcept;
    
protected:
    bool isNewNextFile() noexcept override;

private:
    MC_DECL_PRIVATE(McSizeRollingFileAppender)
};

MC_DECL_METATYPE(McSizeRollingFileAppender)
