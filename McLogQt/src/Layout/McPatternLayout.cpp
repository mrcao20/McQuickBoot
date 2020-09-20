#include "McLog/Layout/impl/McPatternLayout.h"

#include "McLog/Utils/McMessagePattern.h"

MC_DECL_PRIVATE_DATA(McPatternLayout)
QString pattern;
McPrivate::McMessagePatternPtr messagePattern;
MC_DECL_PRIVATE_DATA_END

MC_INIT(McPatternLayout)
MC_REGISTER_BEAN_FACTORY(MC_TYPELIST(McPatternLayout))
MC_INIT_END

McPatternLayout::McPatternLayout() 
{
    MC_NEW_PRIVATE_DATA(McPatternLayout);
    
    d->messagePattern = McPrivate::McMessagePatternPtr::create();
}

McPatternLayout::~McPatternLayout() 
{
}

QString McPatternLayout::getPattern() const noexcept 
{
    return d->pattern;
}

void McPatternLayout::setPattern(const QString &val) noexcept 
{
    d->pattern = val;
    
    d->messagePattern->setPattern(val);
}

QString McPatternLayout::format(QtMsgType type, const QMessageLogContext &context, const QString &str) noexcept 
{
    return McPrivate::format(d->messagePattern, type, context, str);
}

void McPatternLayout::finished() noexcept 
{
}
