#include "McLog/Layout/impl/McTTCCLayout.h"

MC_INIT(McTTCCLayout)
MC_REGISTER_BEAN_FACTORY(McTTCCLayout)
MC_INIT_END

void McTTCCLayout::finished() noexcept 
{
    McPatternLayout::finished();
    
    QString pattern = "[%{appname}][%{time yyyy-MM-dd hh:mm:ss,zzz}][%{category}][%{type}]: "
                      "%{message}                                                           "
                      "[File:%{file}] [Line:%{line}] [Function:%{function}] "
                      "[pid:%{pid}] [threadid:%{threadid}] [qthreadptr:%{qthreadptr}]"
                      "[process:%{time process}] [boot:%{time boot}]";
    setPattern(pattern);
}
