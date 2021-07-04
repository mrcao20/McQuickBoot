#pragma once

#include "McSizeRollingFileAppender.h"

MC_FORWARD_DECL_PRIVATE_DATA(McBySizeDayRollingFileAppender)

class MCLOGQT_EXPORT McBySizeDayRollingFileAppender : public McSizeRollingFileAppender
{
    Q_OBJECT
    MC_DECL_INIT(McBySizeDayRollingFileAppender)
    MC_TYPELIST(McSizeRollingFileAppender)
    Q_PROPERTY(int day READ day WRITE setDay)
public:
    McBySizeDayRollingFileAppender();
    ~McBySizeDayRollingFileAppender();

    int day() const noexcept;
    void setDay(int val) noexcept;

protected:
    bool isNewNextFile() noexcept override;

private:
    MC_DECL_PRIVATE(McBySizeDayRollingFileAppender)
};

MC_DECL_METATYPE(McBySizeDayRollingFileAppender)
