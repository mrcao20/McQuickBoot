#ifndef ARTICLE_H
#define ARTICLE_H

#include <QObject>
#include <McOrm/McOrmGlobal.h>

struct Article : public QObject
{
    Q_OBJECT
    MC_DECL_INIT(Article)
    
    MC_PRIMARY_KEY("id")
    MC_POCO_PROPERTY(int, id)
};

MC_DECL_METATYPE(Article)

#endif // ARTICLE_H
