#pragma once

#include "../../McGlobal.h"

class MCIOCCONTAINER_EXPORT McBeanConnector : public QObject {
    Q_OBJECT
    MC_DECL_INIT(McBeanConnector)
public:
    explicit McBeanConnector(QObject *parent = nullptr)
        : QObject(parent)
    {}
    ~McBeanConnector() override = default;

    QString getSender() const noexcept 
    { return m_sender; }
    void setSender(const QString &sender) noexcept
    { m_sender = sender.simplified(); }
     
    QString getSignal() const noexcept 
    { return m_signal; }
    void setSignal(const QString &signal) noexcept
    { m_signal = QMetaObject::normalizedSignature(signal.toLocal8Bit()); }
    
    QString getReceiver() const noexcept 
    { return m_receiver; }
    void setReceiver(const QString &receiver) noexcept
    { m_receiver = receiver.simplified(); }
    
    QString getSlot() const noexcept 
    { return m_slot; }
    void setSlot(const QString &slot) noexcept
    { m_slot = QMetaObject::normalizedSignature(slot.toLocal8Bit()); }
    
    Qt::ConnectionType getType() const noexcept 
    { return m_type; }
    void setType(const Qt::ConnectionType &type) noexcept
    { m_type = type; }
    
private:
    QString m_sender{MC_THIS_TAG};                      //!< 发送方beanName，默认为对象本身
    QString m_signal;                               //!< 信号名
    QString m_receiver{MC_THIS_TAG};                    //!< 接收方beanName，默认为对象本身
    QString m_slot;                                 //!< 槽名
    Qt::ConnectionType m_type{Qt::AutoConnection};  //!< 连接方式，默认为自动连接
};

MC_DECL_METATYPE(McBeanConnector)
