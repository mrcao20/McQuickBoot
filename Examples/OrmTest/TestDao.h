#ifndef TESTDAO_H
#define TESTDAO_H

#include <QObject>
#include <McOrm/McOrmGlobal.h>
#include "TestVo.h"

class TestDao : public QObject
{
    Q_OBJECT
    MC_DECL_INIT(TestDao)
    MC_DAO
signals:
    TestVoPtr selectById(int id);
};

MC_DECL_METATYPE(TestDao)

#endif // TESTDAO_H
