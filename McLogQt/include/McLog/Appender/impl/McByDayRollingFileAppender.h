#pragma once

#include "McRollingFileAppender.h"

MC_FORWARD_DECL_PRIVATE_DATA(McByDayRollingFileAppender)

class MCLOGQT_EXPORT McByDayRollingFileAppender : public McRollingFileAppender
{
    Q_OBJECT
    MC_DECL_INIT(McByDayRollingFileAppender)
    MC_TYPELIST(McRollingFileAppender)
    Q_PROPERTY(int day READ day WRITE setDay)
public:
    McByDayRollingFileAppender();
    ~McByDayRollingFileAppender();

    int day() const noexcept;
    void setDay(int val) noexcept;

protected:
    bool isNewNextFile() noexcept override;

private:
    MC_DECL_PRIVATE(McByDayRollingFileAppender)
};

MC_DECL_METATYPE(McByDayRollingFileAppender)
