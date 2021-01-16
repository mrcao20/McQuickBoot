#pragma once

#include "../../McLogGlobal.h"
#include "../../Repository/IMcAdditionalTask.h"

MC_FORWARD_DECL_PRIVATE_DATA(McLogPackager);

class MCLOGQT_EXPORT McLogPackager : public QObject, public IMcAdditionalTask
{
    Q_OBJECT
    MC_TYPELIST(IMcAdditionalTask)
    Q_PRIVATE_PROPERTY(d, QList<QString> scanPaths MEMBER scanPaths)
    Q_PRIVATE_PROPERTY(d, int maxDepth MEMBER maxDepth)
    Q_PRIVATE_PROPERTY(d, QString age MEMBER age)
    Q_PRIVATE_PROPERTY(d, QList<QString> filters MEMBER filters)
    Q_PRIVATE_PROPERTY(d, QString dstPath MEMBER dstPath)
    Q_PRIVATE_PROPERTY(d, QString fileNamePattern MEMBER fileNamePattern)
public:
    Q_INVOKABLE McLogPackager() noexcept;
    ~McLogPackager();

    Q_INVOKABLE
    MC_BEAN_FINISHED
    void finished() noexcept;

    void execute() noexcept override;

private:
    QStringList checkFiles(int depth, const QString &path) noexcept;
    bool fileNameCheck(const QString &fileName) noexcept;
    QString newFilePath() const noexcept;

private:
    MC_DECL_PRIVATE(McLogPackager)
};

MC_DECL_METATYPE(McLogPackager)

