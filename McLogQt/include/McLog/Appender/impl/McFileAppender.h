#pragma once

#include "McFileDeviceAppender.h"

MC_FORWARD_DECL_PRIVATE_DATA(McFileAppender);

class MCLOGQT_EXPORT McFileAppender : public McFileDeviceAppender 
{
    Q_OBJECT
    MC_DECL_INIT(McFileAppender)
    MC_TYPELIST(McFileDeviceAppender)
    Q_PROPERTY(QString dirPath READ dirPath WRITE setDirPath)
    Q_PROPERTY(QString fileNamePattern READ fileNamePattern WRITE setFileNamePattern)
    Q_PROPERTY(bool append READ isAppend WRITE setAppend)
public:
    Q_INVOKABLE McFileAppender();
    ~McFileAppender() override;
    
    QString dirPath() const noexcept;
    void setDirPath(const QString &val) noexcept;
    
    QString fileNamePattern() const noexcept;
    void setFileNamePattern(const QString &val) noexcept;
    
    bool isAppend() const noexcept;
    void setAppend(bool val) noexcept;
    
    void finished() noexcept override;
    
protected:
    QString newFilePath() const noexcept;
    
private:
    MC_DECL_PRIVATE(McFileAppender)
};

MC_DECL_METATYPE(McFileAppender)
