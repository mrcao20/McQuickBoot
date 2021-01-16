#include "McLog/Utils/Packager/McLogPackager.h"

#include <QDateTime>
#include <QDir>
#include <QRegularExpression>
#include <QTimer>

#include <McIoc/Utils/Zip/McCompressor.h>

MC_STATIC()
MC_REGISTER_BEAN_FACTORY(McLogPackager)
MC_STATIC_END

MC_DECL_PRIVATE_DATA(McLogPackager)
QList<QString> scanPaths;
int maxDepth{1};
QString age{"5D"};
int ageNumeric{5};
QList<QString> filters;
QString dstPath;
QString fileNamePattern;
MC_DECL_PRIVATE_DATA_END

McLogPackager::McLogPackager() noexcept
{
    MC_NEW_PRIVATE_DATA(McLogPackager);
}

McLogPackager::~McLogPackager()
{
}

void McLogPackager::finished() noexcept
{
    if (!d->age.endsWith("D", Qt::CaseInsensitive)) {
        MC_PRINT_ERR("age must endsWith 'D' for McLogDeleter property\n");
        return;
    }
    auto tmp = d->age;
    tmp.remove(tmp.length() - 1, 1);
    bool isOk = false;
    auto num = tmp.toInt(&isOk);
    if (isOk) {
        d->ageNumeric = num;
    }
}

void McLogPackager::execute() noexcept
{
    QStringList filePaths;
    for (auto &scanPath : qAsConst(d->scanPaths)) {
        auto path = Mc::toAbsolutePath(scanPath);
        filePaths.append(checkFiles(0, path));
    }
    if (filePaths.isEmpty()) {
        return;
    }
    auto tarPath = newFilePath();
    if (!McCompressor::compressFiles(tarPath, filePaths)) {
        return;
    }
    for (auto filePath : filePaths) {
        qInfo() << "The file has packed. deleted:" << filePath;
        QFile::remove(filePath);
    }
}

QStringList McLogPackager::checkFiles(int depth, const QString &path) noexcept
{
    auto curDepth = depth + 1;
    if (curDepth > d->maxDepth) {
        return QStringList();
    }
    QStringList filePaths;
    QDir dir(path);
    auto fileInfos = dir.entryInfoList(QDir::Files | QDir::Dirs | QDir::NoDotAndDotDot);
    for (auto &fileInfo : qAsConst(fileInfos)) {
        auto absPath = fileInfo.absoluteFilePath();
        if (fileInfo.isDir()) {
            filePaths.append(checkFiles(curDepth, absPath));
            continue;
        }
        if (!fileNameCheck(fileInfo.fileName())) {
            continue;
        }
        auto fileDate = fileInfo.lastModified().date();
        auto curDate = QDate::currentDate();
        auto day = fileDate.daysTo(curDate);
        if (day > d->ageNumeric) {
            filePaths.append(absPath);
        }
    }
    return filePaths;
}

bool McLogPackager::fileNameCheck(const QString &fileName) noexcept
{
    for (auto &filter : qAsConst(d->filters)) {
        QRegularExpression reg(filter);
        auto match = reg.match(fileName);
        if (!match.capturedTexts().isEmpty()) {
            return true;
        }
    }
    return false;
}

QString McLogPackager::newFilePath() const noexcept
{
    QRegularExpression re(R"((.*)%\{time (.*?)\}(.*))");
    auto match = re.match(d->fileNamePattern);

    QDir dir(d->dstPath);
    if (!match.hasMatch()) {
        return dir.absoluteFilePath(d->fileNamePattern);
    }

    auto list = match.capturedTexts();
    return dir.absoluteFilePath(list.at(1) + QDateTime::currentDateTime().toString(list.at(2))
                                + list.at(3));
}

#include "moc_McLogPackager.cpp"
