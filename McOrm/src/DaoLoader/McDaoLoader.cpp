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
#include "McOrm/DaoLoader/impl/McDaoLoader.h"

#include <QDebug>

#include <McIoc/ApplicationContext/impl/McAnnotationApplicationContext.h>

#include "McOrm/Parser/impl/McSelectParser.h"

MC_INIT(McDaoLoader)
MC_REGISTER_BEAN_FACTORY(McDaoLoader)
MC_INIT_END

MC_DECL_PRIVATE_DATA(McDaoLoader)
QList<IMcParserPtr> parsers;
MC_DECL_PRIVATE_DATA_END

McDaoLoader::McDaoLoader() noexcept
{
    MC_NEW_PRIVATE_DATA(McDaoLoader);
    
    d->parsers.append(McSelectParserPtr::create());
}

McDaoLoader::~McDaoLoader()
{
}

void McDaoLoader::init() noexcept
{
    McAnnotationApplicationContextPtr appCtx = McAnnotationApplicationContextPtr::create();
    init(appCtx);
}

void McDaoLoader::init(IMcApplicationContextConstPtrRef appCtx) noexcept
{
    auto daoNames = Mc::getComponents(appCtx, MC_DAO_TAG);
    QList<QObjectPtr> daos;
    for(auto name : daoNames) {
        auto dao = appCtx->getBean(name);
        if(dao.isNull()) {
            continue;
        }
        daos.append(dao);
    }
    for(auto parser : d->parsers) {
        parser->parse(daos);
    }
}
