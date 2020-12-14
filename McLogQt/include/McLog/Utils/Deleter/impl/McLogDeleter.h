#pragma once

#include "../IMcLogDeleter.h"
#include "../../../McLogGlobal.h"

MC_FORWARD_DECL_PRIVATE_DATA(McLogDeleter);

class MCLOGQT_EXPORT McLogDeleter : public QObject, public IMcLogDeleter
{
    Q_OBJECT
    MC_DEFINE_TYPELIST(IMcLogDeleter);
    Q_PRIVATE_PROPERTY(d, QString basePath MEMBER basePath)
    Q_PRIVATE_PROPERTY(d, int maxDepth MEMBER maxDepth)
    Q_PRIVATE_PROPERTY(d, QString age MEMBER age)
    Q_PRIVATE_PROPERTY(d, QList<QString> filters MEMBER filters)
public:
    Q_INVOKABLE explicit McLogDeleter(QObject *parent = nullptr) noexcept;
    ~McLogDeleter() override;
    
    Q_INVOKABLE
    MC_BEAN_FINISHED
    void finished() noexcept;
    
private slots:
    void check() noexcept;
    
private:
    void checkFiles(int depth, const QString &path) noexcept;
    bool fileNameCheck(const QString &fileName) noexcept;
    
private:
    MC_DECL_PRIVATE(McLogDeleter)
};

MC_DECL_METATYPE(McLogDeleter)

