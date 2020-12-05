#ifndef TESTVO_H
#define TESTVO_H

#include <QObject>
#include <McOrm/McOrmGlobal.h>

MC_FORWARD_DECL_STRUCT(Article);

struct TestVo : public QObject
{
    Q_OBJECT
    MC_DECL_INIT(TestVo)
    
    //! 表名
    MC_DB_TABLE("t_user");
    
public:
    Q_INVOKABLE TestVo() {}
    
    //! 表明此结构体中的属性id为表中的主键
    MC_PRIMARY_KEY("id")
    //! 此结构体中的属性id和表中的字段user_id绑定
    MC_TABLE_COL("id = user_id")
    MC_POCO_PROPERTY(int, id, {-1})
    
    //! ...
    MC_TABLE_COL("userName = username")
    MC_POCO_PROPERTY(QString, userName)
    
    //! 没有显式绑定，那么此结构体中的属性password与表中同名字段password绑定
    MC_POCO_PROPERTY(QString, password)
    
    //! 表明此结构体中的属性articles依赖一个外键，此外键为Article结构体中的属性id，但具体是表中的哪个字段由Article指代，此处只和结构体中的属性相绑定
    MC_FOREIGN_KEY("article = id", MC_ONE_TO_ONE())
    //! 此结构体中的属性articles与表中的字段article_id相绑定，查找出的article_id会再去Article对应表中查找Article，然后填充到该属性
    MC_TABLE_COL("article = article_id")
    MC_POCO_PROPERTY(ArticlePtr, article)
};

MC_DECL_METATYPE(TestVo)

#endif // TESTVO_H
