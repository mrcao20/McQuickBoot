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
#include "McLog/Layout/impl/McNormalLayout.h"

MC_INIT(McNormalLayout)
MC_REGISTER_BEAN_FACTORY(McNormalLayout)
MC_INIT_END

void McNormalLayout::finished() noexcept 
{
    McPatternLayout::finished();

    QString pattern = "[%{time yyyy-MM-dd hh:mm:ss,zzz}][%{category}]"
                      "[%{if-debug}DEBUG]:   %{endif}"
#if QT_VERSION >= QT_VERSION_CHECK(5, 5, 0)
                      "%{if-info}INFO]:    %{endif}"
#endif
                      "%{if-warning}WARN]:    %{endif}%{if-critical}CRITICAL]:%{endif}"
                      "%{if-fatal}FATAL]:   %{endif} "
                      "%{message}                                                      "
                      "[File:%{file}] [Line:%{line}] [Function:%{function}]";
    setPattern(pattern);
}
