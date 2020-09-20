#pragma once

#include <QElapsedTimer>

#include "../McLogGlobal.h"

namespace McPrivate {

struct McMessagePattern 
{
    McMessagePattern();
    ~McMessagePattern();

    void setPattern(const QString &pattern);

    // 0 terminated arrays of literal tokens / literal or placeholder tokens
    const char **literals;
    const char **tokens;
    QList<QString> timeArgs;   // timeFormats in sequence of %{time
#ifndef QT_BOOTSTRAPPED
    QElapsedTimer timer;
#endif
#ifdef QLOGGING_HAVE_BACKTRACE
    struct BacktraceParams 
    {
        QString backtraceSeparator;
        int backtraceDepth;
    };
    QVector<BacktraceParams> backtraceArgs; // backtrace argumens in sequence of %{backtrace
#endif
};
MC_DECL_POINTER(McMessagePattern)
#ifdef QLOGGING_HAVE_BACKTRACE
Q_DECLARE_TYPEINFO(McMessagePattern::BacktraceParams, Q_MOVABLE_TYPE);
#endif

QString format(McMessagePatternPtr pattern, QtMsgType type, const QMessageLogContext &context, const QString &str) noexcept;

}
