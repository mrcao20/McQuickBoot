/*
 * Copyright (c) 2021 mrcao20/mrcao20@163.com
 * McQuickBoot is licensed under Mulan PSL v2.
 * You can use this software according to the terms and conditions of the Mulan PSL v2.
 * You may obtain a copy of Mulan PSL v2 at:
 *          http://license.coscl.org.cn/MulanPSL2
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND,
 * EITHER EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT,
 * MERCHANTABILITY OR FIT FOR A PARTICULAR PURPOSE.
 * See the Mulan PSL v2 for more details.
 */
#include "McTTCCLayout.h"

MC_AUTO_INIT(McTTCCLayout)
MC_INIT_END

void McTTCCLayout::buildFinished() noexcept
{
    super::buildFinished();

    QString pattern = "[%{appname}][%{time yyyy-MM-dd hh:mm:ss,zzz}][%{category}][%{type}]: "
                      "%{message}                                                           "
                      "[File:%{file}] [Line:%{line}] [Function:%{function}] "
                      "[pid:%{pid}] [threadid:%{threadid}] [qthreadptr:%{qthreadptr}]"
                      "[process:%{time process}] [boot:%{time boot}]";
    setPattern(pattern);
}
