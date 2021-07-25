#pragma once

#include <QObject>
#include <QSharedData>

#include <McIoc/McGlobal.h>

#include "../McBootMacroGlobal.h"

struct MCQUICKBOOT_EXPORT McCancelSharedData : public QSharedData
{
    QAtomicInteger<bool> isCanceled{false};
};

class MCQUICKBOOT_EXPORT McCancel
{
    MC_DECL_INIT(McCancel)
public:
    McCancel() noexcept;
    ~McCancel();

    bool isCanceled() const noexcept;
    void cancel() noexcept;

private:
    QExplicitlySharedDataPointer<McCancelSharedData> d;
};

Q_DECLARE_METATYPE(McCancel)
