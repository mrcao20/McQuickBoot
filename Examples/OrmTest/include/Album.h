#pragma once

#include <McOrm/McOrmGlobal.h>

struct Album : public QObject
{
    Q_OBJECT

    MC_DB_TABLE("t_album_detail_info")
public:
    Q_INVOKABLE Album() {}

    MC_PRIMARY_KEY("id")
    MC_TABLE_COL("id", "album_index")
    MC_POCO_PROPERTY(int, id)

    MC_TABLE_COL("albumId", "album_id")
    MC_POCO_PROPERTY(QString, albumId)

    MC_TABLE_COL("albumMid", "album_mid")
    MC_POCO_PROPERTY(QString, albumMid)

    MC_TABLE_COL("albumName", "album_name")
    MC_POCO_PROPERTY(QString, albumName)

    MC_TABLE_COL("albumTitle", "album_title")
    MC_POCO_PROPERTY(QString, albumTitle)

    MC_TABLE_COL("albumTitleHilight", "album_title_hilight")
    MC_POCO_PROPERTY(QString, albumTitleHilight)
};

MC_DECL_METATYPE(Album)

