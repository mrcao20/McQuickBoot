/*
 * MIT License
 *
 * Copyright (c) 2021 mrcao20
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */
#pragma once

#include <McIoc/McGlobal.h>

#define MC_DAO_TAG "__McDao__"

#define MC_DAO(...) \
    MC_BEANNAME("" __VA_ARGS__) \
    Q_CLASSINFO(MC_COMPONENT_TAG, MC_DAO_TAG)

#define MC_DB_TABLE_TAG "dbTable"
#define MC_TABLE_COL_TAG "tableColumn"
#define MC_PRIMARY_KEY_TAG "primaryKey"
#define MC_FOREIGN_KEY_TAG "foreignKey"
#define MC_NORMAL_SPLIT_SYMBOL "="

#define MC_DB_TABLE(name) Q_CLASSINFO(MC_DB_TABLE_TAG, name)
#define MC_TABLE_COL(name, col) Q_CLASSINFO(MC_TABLE_COL_TAG, name MC_NORMAL_SPLIT_SYMBOL col)
#define MC_PRIMARY_KEY(arg) Q_CLASSINFO(MC_PRIMARY_KEY_TAG, arg)
#define MC_FOREIGN_KEY(arg, arg2, ...) \
    Q_CLASSINFO(MC_FOREIGN_KEY_TAG, arg MC_NORMAL_SPLIT_SYMBOL arg2 __VA_ARGS__)

#define MC_MAPPED_SPLIT_SYMBOL "@"
#define MC_ONE_TO_ONE_TAG "oneToOne"
#define MC_ONE_TO_MANY_TAG "oneToMany"
#define MC_MANY_TO_ONE_TAG "manyToOne"
#define MC_MANY_TO_MANY_TAG "manyToMany"

#define MC_ONE_TO_ONE(...) MC_MAPPED_SPLIT_SYMBOL MC_ONE_TO_ONE_TAG __VA_ARGS__
#define MC_ONE_TO_MANY(...) MC_MAPPED_SPLIT_SYMBOL MC_ONE_TO_MANY_TAG __VA_ARGS__
#define MC_MANY_TO_ONE(...) MC_MAPPED_SPLIT_SYMBOL MC_MANY_TO_ONE_TAG __VA_ARGS__
#define MC_MANY_TO_MANY(...) MC_MAPPED_SPLIT_SYMBOL MC_MANY_TO_MANY_TAG __VA_ARGS__


#define MC_SQL_SPLIT_SYMBOL "@"
#define MC_SQL(sql) MC_SQL_SPLIT_SYMBOL sql

#define MC_SELECT_TAG "dbSelect"
#define MC_DELETE_TAG "dbDelete"
#define MC_UPDATE_TAG "dbUpdate"
#define MC_INSERT_TAG "dbInsert"

#define MC_SELECT(name, ...) \
    Q_CLASSINFO(MC_SELECT_TAG, name __VA_ARGS__) \
    Q_SIGNAL
#define MC_DELETE(name, ...) \
    Q_CLASSINFO(MC_DELETE_TAG, name __VA_ARGS__) \
    Q_SIGNAL
#define MC_UPDATE(name, ...) \
    Q_CLASSINFO(MC_UPDATE_TAG, name __VA_ARGS__) \
    Q_SIGNAL
#define MC_INSERT(name, ...) \
    Q_CLASSINFO(MC_INSERT_TAG, name __VA_ARGS__) \
    Q_SIGNAL
