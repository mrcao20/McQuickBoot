#include "McLog/Appender/impl/McAbstractAppender.h"

#define LEVEL_DEBUG "debug"
#define LEVEL_WARN "warn"
#define LEVEL_CRITICAL "critical"
#define LEVEL_FATAL "fatal"
#define LEVEL_INFO "info"

MC_DECL_PRIVATE_DATA(McAbstractAppender)
QString threshold;                  //!< 全小写
QList<QtMsgType> types;
MC_PADDING_CLANG(7)
MC_DECL_PRIVATE_DATA_END

MC_INIT(McAbstractAppender)
MC_INIT_END

McAbstractAppender::McAbstractAppender() 
{
    MC_NEW_PRIVATE_DATA(McAbstractAppender);
}

McAbstractAppender::~McAbstractAppender() 
{
}

QString McAbstractAppender::threshold() const noexcept 
{
    return d->threshold;
}

void McAbstractAppender::setThreshold(const QString &val) noexcept 
{
    d->threshold = val.toLower();
    
    d->types = initThreshold(d->threshold);
}

void McAbstractAppender::finished() noexcept 
{
}

void McAbstractAppender::threadFinished() noexcept 
{
}

QList<QtMsgType> McAbstractAppender::types() const noexcept
{
    return d->types;
}

QList<QtMsgType> McAbstractAppender::initThreshold(const QString &val) const noexcept 
{
    auto typeStr = val.simplified();
    QList<QtMsgType> types;
    
    if(!typeStr.endsWith('-') && !typeStr.startsWith('-')) {
        auto list = typeStr.split(',');
        for(auto str : list) {
            auto type = strToEnum(str.simplified());
            if(type != -1) {
                types.append(static_cast<QtMsgType>(type));
            }
        }
        return types;
    }else if(typeStr.startsWith('-')) {
        auto level = typeStr.simplified();
        level.remove('-');
        auto type = strToEnum(level);
        
        types.append(static_cast<QtMsgType>(type));
        if (level == LEVEL_DEBUG)
            return types;
        types.append(QtMsgType::QtDebugMsg);
        if (level == LEVEL_WARN)
            return types;
        types.append(QtMsgType::QtWarningMsg);
        if (level == LEVEL_CRITICAL)
            return types;
        types.append(QtMsgType::QtCriticalMsg);
        if (level == LEVEL_FATAL)
            return types;
        types.append(QtMsgType::QtFatalMsg);
        if (level != LEVEL_INFO) {
            types.clear();
            return types;
        }
        
        return types;
    }else{
        auto level = typeStr.simplified();
        level.remove('-');
        auto type = strToEnum(level);
        
        types.append(static_cast<QtMsgType>(type));
#if QT_VERSION >= QT_VERSION_CHECK(5, 5, 0)
        if (level == LEVEL_INFO)
            return types;
        types.append(QtMsgType::QtInfoMsg);
#endif
        if (level == LEVEL_FATAL)
            return types;
        types.append(QtMsgType::QtFatalMsg);
        if (level == LEVEL_CRITICAL)
            return types;
        types.append(QtMsgType::QtCriticalMsg);
        if (level == LEVEL_WARN)
            return types;
        types.append(QtMsgType::QtWarningMsg);
        if (level != LEVEL_DEBUG) {
            types.clear();
            return types;
        }
        
        return types;
    }
}

int McAbstractAppender::strToEnum(const QString &val) const noexcept 
{
    auto type = val.simplified();
    if (type == LEVEL_DEBUG)
        return QtMsgType::QtDebugMsg;
    else if (type == LEVEL_WARN)
        return QtMsgType::QtWarningMsg;
    else if (type == LEVEL_CRITICAL)
        return QtMsgType::QtCriticalMsg;
    else if (type == LEVEL_FATAL)
        return QtMsgType::QtFatalMsg;
#if QT_VERSION >= QT_VERSION_CHECK(5, 5, 0)
    else if(type == LEVEL_INFO)
        return QtMsgType::QtInfoMsg;
#endif
    return -1;
}
