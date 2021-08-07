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
#include "McOrm/Parser/impl/McSelectParser.h"

#include "McOrm/McOrmGlobal.h"
#include "McOrm/SqlSlot/impl/McSelectSqlSlot.h"

MC_INIT(McSelectParser)
MC_REGISTER_BEAN_FACTORY(McSelectParser)
MC_INIT_END

MC_DECL_PRIVATE_DATA(McSelectParser)
MC_DECL_PRIVATE_DATA_END

McSelectParser::McSelectParser() noexcept
{
    MC_NEW_PRIVATE_DATA(McSelectParser);
}

McSelectParser::~McSelectParser()
{
}

IMcSqlSlot *McSelectParser::createSqlSlot(const char *tag) noexcept
{
    if(qstrcmp(tag, MC_SELECT_TAG) != 0) {
        return nullptr;
    }
    return new McSelectSqlSlot();
}
