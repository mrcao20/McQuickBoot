#include "McLog/Appender/impl/McFileAppender.h"

#include <QCoreApplication>
#include <QFile>
#include <QDir>
#include <QFileInfo>
#include <QRegularExpression>
#include <QRegularExpressionMatch>
#include <QDateTime>

MC_DECL_PRIVATE_DATA(McFileAppender)
QString dirPath;
QString fileNamePattern;
bool isAppend{true};
MC_PADDING_CLANG(7)
MC_DECL_PRIVATE_DATA_END

MC_INIT(McFileAppender)
MC_REGISTER_BEAN_FACTORY(McFileAppender)
MC_INIT_END

McFileAppender::McFileAppender() 
{
    MC_NEW_PRIVATE_DATA(McFileAppender);
}

McFileAppender::~McFileAppender() 
{
}

QString McFileAppender::dirPath() const noexcept 
{
    return d->dirPath;
}

void McFileAppender::setDirPath(const QString &val) noexcept 
{
    d->dirPath = Mc::toAbsolutePath(val);
}

QString McFileAppender::fileNamePattern() const noexcept 
{
    return d->fileNamePattern;
}

void McFileAppender::setFileNamePattern(const QString &val) noexcept 
{
    d->fileNamePattern = val;
}

bool McFileAppender::isAppend() const noexcept 
{
    return d->isAppend;
}

void McFileAppender::setAppend(bool val) noexcept 
{
    d->isAppend = val;
}

void McFileAppender::finished() noexcept 
{
    McFileDeviceAppender::finished();
    
    QSharedPointer<QFile> file = QSharedPointer<QFile>::create();
    setDevice(file);
    
    QRegularExpression re(R"((.*)%\{time (.*?)\}(.*))");
    auto match = re.match(d->fileNamePattern);
    
    QDir dir(d->dirPath);
    if(!dir.exists() && !dir.mkpath(d->dirPath)) {
        MC_PRINT_ERR("the dir path: %s is not exists. but cannot create!\n", qPrintable(d->dirPath));
        return;
    }
    auto fileInfos = dir.entryInfoList(QDir::Files, QDir::Time);    //!< 获取最新被修改的文件
    QString localFilePath;
    for(auto fileInfo : fileInfos) {
        auto fileName = fileInfo.fileName();
        if(match.hasMatch()) {  //!< 如果匹配成功，则list的长度一定为4
            auto list = match.capturedTexts();
            if(!fileName.startsWith(list.at(1))
                    || !fileName.endsWith(list.at(3))) {
                continue;
            }
            fileName.remove(fileName.size() - list.at(3).size(), list.at(3).size()); //!< 移除末尾
            fileName.remove(0, list.at(1).size()); //!< 移除开头
            QDateTime dateTime = QDateTime::fromString(fileName, list.at(2));
            if(!dateTime.isValid()) {
                continue;
            }
            localFilePath = fileInfo.filePath();
            break;
        }else{
            if(d->fileNamePattern != fileName) {
                continue;
            }
            localFilePath = fileInfo.filePath();
            break;
        }
    }
    
    if(localFilePath.isEmpty()) {
        localFilePath = newFilePath();
    }
    
    file->setFileName(localFilePath);
    QIODevice::OpenMode mode = QIODevice::WriteOnly | QIODevice::Text;
    if (d->isAppend)
        mode |= QIODevice::Append;
    file->open(mode);
    
    tryNextFile();
}

QString McFileAppender::newFilePath() const noexcept 
{
    QRegularExpression re(R"((.*)%\{time (.*?)\}(.*))");
    auto match = re.match(d->fileNamePattern);
    
    QDir dir(d->dirPath);
    if(!match.hasMatch()) {
        return dir.absoluteFilePath(d->fileNamePattern);
    }
    
    auto list = match.capturedTexts();
    return dir.absoluteFilePath(list.at(1) 
                                + QDateTime::currentDateTime().toString(list.at(2)) 
                                + list.at(3));
}
