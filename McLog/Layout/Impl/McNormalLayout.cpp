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
#include "McNormalLayout.h"

MC_AUTO_INIT(McNormalLayout)
MC_INIT_END

void McNormalLayout::buildFinished() noexcept
{
    super::buildFinished();

    QString pattern = QStringLiteral("[%{time yyyy-MM-dd hh:mm:ss,zzz}][%{category}]"
                                     "[%{if-debug}DEBUG]:   %{endif}"
                                     "%{if-info}INFO]:    %{endif}"
                                     "%{if-warning}WARN]:    %{endif}"
                                     "%{if-critical}CRITICAL]:%{endif}"
                                     "%{if-fatal}FATAL]:   %{endif} "
                                     "%{message}                                                      "
                                     "[File:%{file}] [Line:%{line}] [Function:%{function}]");
    setPattern(pattern);
}
