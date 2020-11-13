#ifndef TESTDAO_H
#define TESTDAO_H

#include <QObject>
#include <McOrm/McOrmGlobal.h>
#include "TestVo.h"
#include "Article.h"

class TestDao : public QObject
{
    Q_OBJECT
    MC_DECL_INIT(TestDao)
    MC_DAO
    MC_BEANNAME("testDao")
public:
    Q_INVOKABLE TestDao(){}
    
    MC_SELECT("selectById")
    TestVoPtr selectById(int id);
    
    MC_SELECT("selectByArticleId")
    ArticlePtr selectByArticleId(int id);
};

MC_DECL_METATYPE(TestDao)

#endif // TESTDAO_H
