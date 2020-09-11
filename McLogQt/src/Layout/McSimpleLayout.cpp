#include "McLog/Layout/impl/McSimpleLayout.h"

MC_INIT(McSimpleLayout)
MC_REGISTER_BEAN_FACTORY(MC_TYPELIST(McSimpleLayout))
MC_INIT_END

void McSimpleLayout::finished() noexcept 
{
    McPatternLayout::finished();
    
    QString pattern = "[%{type}]: %{message}";
    setPattern(pattern);
}
