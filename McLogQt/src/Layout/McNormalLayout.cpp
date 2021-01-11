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
