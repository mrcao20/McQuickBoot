#pragma once

#include <QIODevice>
#include <McIoc/McGlobal.h>

MC_FORWARD_DECL_PRIVATE_DATA(McVSDebugDevice);

class McVSDebugDevice : public QIODevice
{
    Q_OBJECT
    MC_DECL_INIT(McVSDebugDevice)
    MC_DEFINE_TYPELIST();
public:
    Q_INVOKABLE explicit McVSDebugDevice(QObject *parent = nullptr) noexcept;
    ~McVSDebugDevice() override;
    
protected:
    qint64 readData(char *data, qint64 maxSize) override;
    qint64 writeData(const char *data, qint64 maxSize) override;
    
private:
    MC_DECL_PRIVATE(McVSDebugDevice)
};

MC_DECL_METATYPE(McVSDebugDevice)

