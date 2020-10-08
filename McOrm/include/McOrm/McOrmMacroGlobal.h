#pragma once

#include <McIoc/McGlobal.h>

#define MC_DAO_TAG "Dao"

#define MC_DAO Q_CLASSINFO(MC_COMPONENT_TAG, MC_DAO_TAG)

#define MC_DB_TABLE_TAG "dbTable"
#define MC_TABLE_COL_TAG "tableColumn"
#define MC_PRIMARY_KEY_TAG "primaryKey"
#define MC_FOREIGN_KEY_TAG "foreignKey"

#define MC_DB_TABLE(name) Q_CLASSINFO(MC_DB_TABLE_TAG, name)
#define MC_TABLE_COL(name) Q_CLASSINFO(MC_TABLE_COL_TAG, name)
#define MC_PRIMARY_KEY(arg) Q_CLASSINFO(MC_PRIMARY_KEY_TAG, arg)
#define MC_FOREIGN_KEY(arg) Q_CLASSINFO(MC_FOREIGN_KEY_TAG, arg)
