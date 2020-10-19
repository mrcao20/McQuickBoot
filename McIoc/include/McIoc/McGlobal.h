#pragma once

#include <functional>

#include <QSharedPointer>
#include <QObject>
#include <QEvent>

#include "McIocVersion.h"
#include "BeanFactory/McBeanGlobal.h"

MC_FORWARD_DECL_CLASS(IMcApplicationContext)

namespace McPrivate {

class McGlobal
{
    MC_DECL_INIT(McGlobal)
};

}

MC_DECL_POINTER(QObject)
Q_DECLARE_METATYPE(QObjectPtr)

class MCIOC_EXPORT McCustomEvent : public QEvent {
public:
    McCustomEvent(int type, const QVariant &data) noexcept
        : QEvent(static_cast<QEvent::Type>(type))
        , m_data(data)
    {}
    ~McCustomEvent() noexcept;
    
    QVariant data() const noexcept {
        return m_data;
    }
    void setData(const QVariant &val) noexcept {
        m_data = val;
    }
    
private:
    QVariant m_data;
};

namespace Mc {

template<typename Container>
bool isContains(int index, const Container &container) {
	if (index >= 0 && index < container.size())
		return true;
	return false;
}


/*!
 * \brief mcWaitForExecFunc，执行一个函数，当该函数返回true时或timeout超时时返回
 * 
 * 注意：如果当前线程本身处于退出状态，那么此函数会立即返回
 * 如果timeout设置为-1，则表示永不超时
 * 只有当func返回true时当前函数才会返回true，否则都返回false
 * \param func 将要执行的函数，该函数返回bool值
 * \param timeout 超时时长，单位: ms
 * \return 返回函数执行结果
 */
MCIOC_EXPORT bool waitForExecFunc(
        const std::function<bool()> &func
        , qint64 timeout = -1) noexcept;

/*!
 * \brief mcToAbsolutePath
 * 
 * 如果path为file:///开头的相对路径，那么转为绝对路径。其他格式不变
 * \param path
 * \return 
 */
MCIOC_EXPORT QString toAbsolutePath(const QString &path) noexcept;

//! 获取所有被Component标记的bean
MCIOC_EXPORT QList<QString> getAllComponent(IMcApplicationContextConstPtrRef appCtx) noexcept;
//! 获取所有组建类型为componentType的bean的名称
MCIOC_EXPORT QList<QString> getComponents(IMcApplicationContextConstPtrRef appCtx, const QString &componentType) noexcept;
//! 是否被Component标记
MCIOC_EXPORT bool isComponent(const QMetaObject *metaObj) noexcept;
//! 传入的元对象的组件类型是否为type
MCIOC_EXPORT bool isComponentType(const QMetaObject *metaObj, const QString &type) noexcept;

}
