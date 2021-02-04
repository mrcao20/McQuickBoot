#pragma once

#include <QObject>
#include <QRunnable>

#include "../../McBootGlobal.h"

MC_FORWARD_DECL_PRIVATE_DATA(McRequestRunner);

MC_FORWARD_DECL_CLASS(IMcControllerContainer);

class McAbstractResponse;

class MCQUICKBOOT_EXPORT McRequestRunner : public QObject, public QRunnable
{
    Q_OBJECT
public:
    McRequestRunner();
    ~McRequestRunner() override;

    void setResponse(McAbstractResponse *val) noexcept;
    void setControllerContainer(IMcControllerContainerConstPtrRef val) noexcept;
    void setUri(const QString &uri) noexcept;
    void setBody(const QVariant &body) noexcept;

    void run() override;

signals:
    void signal_finished();

private:
    MC_DECL_PRIVATE(McRequestRunner)
};

MC_DECL_POINTER(McRequestRunner)
