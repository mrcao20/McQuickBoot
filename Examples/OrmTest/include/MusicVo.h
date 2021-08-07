#pragma once

#include "Album.h"
#include <McOrm/McOrmGlobal.h>
#include <QObject>

MC_FORWARD_DECL_PRIVATE_DATA(MusicVo);

class MusicVo : public QObject
{
    Q_OBJECT
    MC_TYPELIST()

    MC_DB_TABLE("t_music_info");

public:
    Q_INVOKABLE MusicVo() noexcept;
    ~MusicVo();

    //! 表明此结构体中的属性id为表中的主键
    MC_PRIMARY_KEY("id")
    //! 此结构体中的属性id和表中的字段user_id绑定
    MC_TABLE_COL("id", "song_index")
    MC_POCO_PROPERTY(int, id, {-1})

    //! ...
    MC_TABLE_COL("songTitle", "song_title")
    MC_POCO_PROPERTY(QString, songTitle)

    MC_TABLE_COL("songUrl", "song_url")
    MC_POCO_PROPERTY(QString, songUrl)

    MC_TABLE_COL("albumIndex", "album_index")
    MC_POCO_PROPERTY(QString, albumIndex)

    //! 表明此结构体中的属性articles依赖一个外键，此外键为Article结构体中的属性id，但具体是表中的哪个字段由Article指代，此处只和结构体中的属性相绑定
    MC_FOREIGN_KEY("album", "id", MC_ONE_TO_ONE())
    //! 此结构体中的属性articles与表中的字段article_id相绑定，查找出的article_id会再去Article对应表中查找Article，然后填充到该属性
    MC_TABLE_COL("album", "album_index")
    MC_POCO_PROPERTY(AlbumPtr, album)

private:
    MC_DECL_PRIVATE(MusicVo)
};

MC_DECL_METATYPE(MusicVo)

