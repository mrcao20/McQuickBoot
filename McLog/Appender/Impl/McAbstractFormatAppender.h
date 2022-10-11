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
#pragma once

#include "McAbstractIODeviceAppender.h"

#ifdef MC_USE_QT6
Q_MOC_INCLUDE("Layout/IMcLayout.h")
#endif

MC_FORWARD_DECL_CLASS(IMcLayout)

MC_FORWARD_DECL_PRIVATE_DATA(McAbstractFormatAppender)

class MC_LOG_EXPORT McAbstractFormatAppender : public McAbstractIODeviceAppender
{
    Q_OBJECT
    MC_DECL_SUPER(McAbstractIODeviceAppender)
    MC_INTERFACES(McAbstractIODeviceAppender)
    Q_PROPERTY(IMcLayoutPtr layout READ layout WRITE setLayout)
    Q_PROPERTY(bool immediateFlush READ immediateFlush WRITE setImmediateFlush)
    Q_PROPERTY(bool useLockFile READ useLockFile WRITE setUseLockFile)
    Q_PROPERTY(bool isPrintError READ isPrintError WRITE setPrintError)
    Q_PROPERTY(QString lockFilePath READ lockFilePath WRITE setLockFilePath)
public:
    McAbstractFormatAppender() noexcept;
    ~McAbstractFormatAppender();

    IMcLayoutPtr layout() const noexcept;
    void setLayout(const IMcLayoutPtr &val) noexcept;

    bool immediateFlush() const noexcept;
    void setImmediateFlush(bool val) noexcept;

    bool useLockFile() const noexcept;
    void setUseLockFile(bool val) noexcept;

    bool isPrintError() const noexcept;
    void setPrintError(bool val) noexcept;

    QString lockFilePath() const noexcept;
    void setLockFilePath(const QString &val) noexcept;

    void append(QtMsgType type, const QMessageLogContext &context, const QString &str) noexcept override;

    void buildFinished() noexcept override;
    void buildThreadMoved() noexcept override;
    void buildCompleted() noexcept override;

protected:
    void customEvent(QEvent *event) override;

    virtual void writeBefore() noexcept = 0;
    virtual void writeAfter() noexcept = 0;

private:
    void append_helper(const QString &msg) noexcept;

private:
    MC_DECL_PRIVATE(McAbstractFormatAppender)
};

MC_DECL_POINTER(McAbstractFormatAppender)
