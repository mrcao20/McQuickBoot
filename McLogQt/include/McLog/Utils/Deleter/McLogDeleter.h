#pragma once

#include "../../McLogGlobal.h"
#include "../../Repository/IMcAdditionalTask.h"

MC_FORWARD_DECL_PRIVATE_DATA(McLogDeleter);

class MCLOGQT_EXPORT McLogDeleter : public QObject, public IMcAdditionalTask
{
    Q_OBJECT
    MC_TYPELIST(IMcAdditionalTask)
    Q_PRIVATE_PROPERTY(d, QList<QString> basePaths MEMBER basePaths)
    Q_PRIVATE_PROPERTY(d, int maxDepth MEMBER maxDepth)
    Q_PRIVATE_PROPERTY(d, QString age MEMBER age)
    Q_PRIVATE_PROPERTY(d, QList<QString> filters MEMBER filters)
public:
    Q_INVOKABLE explicit McLogDeleter(QObject *parent = nullptr) noexcept;
    ~McLogDeleter() override;
    
    Q_INVOKABLE
    MC_BEAN_FINISHED
    void finished() noexcept;

    void execute() noexcept override;

private:
    void checkFiles(int depth, const QString &path) noexcept;
    bool fileNameCheck(const QString &fileName) noexcept;
    
private:
    MC_DECL_PRIVATE(McLogDeleter)
};

MC_DECL_METATYPE(McLogDeleter)

