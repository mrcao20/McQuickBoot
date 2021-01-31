#pragma once

#include "MusicVo.h"
#include <McOrm/McOrmGlobal.h>
#include <QObject>

MC_FORWARD_DECL_PRIVATE_DATA(MusicDao);

class MusicDao : public QObject
{
    Q_OBJECT
    MC_TYPELIST()
    MC_DAO
public:
    Q_INVOKABLE explicit MusicDao(QObject *parent = nullptr) noexcept;
    ~MusicDao() override;

    MC_SELECT("selectById")
    MusicVoPtr selectById(int id);

    MC_SELECT("selectAll")
    QList<MusicVoPtr> selectAll();

private:
    MC_DECL_PRIVATE(MusicDao)
};

MC_DECL_METATYPE(MusicDao)

