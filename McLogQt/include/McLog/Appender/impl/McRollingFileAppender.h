#pragma once

#include "../IMcRequestableNextFile.h"
#include "McFileAppender.h"

MC_FORWARD_DECL_PRIVATE_DATA(McRollingFileAppender);

class MCLOGQT_EXPORT McRollingFileAppender : public McFileAppender, public IMcRequestableNextFile
{
    Q_OBJECT
    MC_DECL_INIT(McRollingFileAppender)
    MC_TYPELIST(McFileAppender, IMcRequestableNextFile)
    Q_PROPERTY(QString backupDirPath READ backupDirPath WRITE setBackupDirPath)
    Q_PROPERTY(QString backupDirPattern READ backupDirPattern WRITE setBackupDirPattern)
public:
    McRollingFileAppender();
    ~McRollingFileAppender() override;
    
    QString backupDirPath() const noexcept;
    void setBackupDirPath(const QString &val) noexcept;
    
    QString backupDirPattern() const noexcept;
    void setBackupDirPattern(const QString &val) noexcept;

    void requestNextFile() noexcept override;
    void forceRequestNextFile() noexcept override;

protected:
    void doFinished() noexcept override;

    void tryNextFile() noexcept override;
    
    virtual bool isNewNextFile() noexcept = 0;
    
private:
    Q_INVOKABLE void nextFile() noexcept;
    QString newBackupPath(const QString &oldFilePath) const noexcept;
    
private:
    MC_DECL_PRIVATE(McRollingFileAppender)
};

MC_DECL_METATYPE(McRollingFileAppender)
