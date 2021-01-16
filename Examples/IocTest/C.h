#pragma once
#include <McIoc/BeanDefinition/IMcCustomPlaceholder.h>
#include <McIoc/McGlobal.h>
#include <QVector>

class GadgetTest
{
    Q_GADGET
    MC_COMPONENT
    MC_BEANNAME("gadgetTest")
public:
    MC_POCO_PROPERTY(int, aaa);
    MC_POCO_PROPERTY(QString, bbb);
};
MC_DECL_METATYPE(GadgetTest)

class R : public QObject, public IMcCustomPlaceholder
{
    Q_OBJECT
    MC_DECL_INIT(R)
    MC_TYPELIST(IMcCustomPlaceholder)
    MC_COMPONENT
    Q_CLASSINFO(MC_BEANNAME_TAG, "r")
    Q_PROPERTY(QString text READ text WRITE setText);
public:
    Q_INVOKABLE R(){}
    
    QString text() const noexcept;
    void setText(const QString &val) noexcept;

    QVariant getKey() const noexcept override;

    Q_INVOKABLE
    MC_BEAN_START
    void start() noexcept;

public slots:
    void slot_recv() noexcept;
    
private:
    QString m_text;
};
MC_DECL_METATYPE(R);

class IA
{
public:
    virtual ~IA() = default;            //!< C++中超类析构函数必须是virtual
    
    virtual void a() noexcept = 0;
};
MC_DECL_METATYPE(IA); //!< 这里必须使用该宏声明，否则无法从C转换到该接口。

class IB : public IA
{
    MC_TYPELIST(
        IA); //!< 由于本接口有一个父接口，并且可能存在从IB转换到IA，所以这里需要使用这个宏保存父接口
public:
};
MC_DECL_METATYPE(IB);

typedef QMap<QString, QString> StringMap;           //!< 由于QMap在Q_PROPERTY宏中有错误提示，所以这里先重定义一下
typedef QHash<QString, QSharedPointer<R>> RHash;
class C : public QObject, public IB
{
    Q_OBJECT
    //!< 这个宏主要用来实现一个类似于java静态代码块的功能。这里只是声明，真正实现在cpp中
    MC_DECL_INIT(C)
    //! 同理，由于C实现至IB接口，并且可能转换到IB，所以这里需要使用该宏。
    //! 这里不需要额外指定QOBject，容器会自动指定。但如果C继承至其他类，比如QWidget，那么需要先使用MC_DECL_POINTER声明QWidget，再使用MC_TYPELIST(QWidget, IB)，
    //! 当然，如果不需要从C转换到QWidget，也就不需要额外声明QWidget
    MC_TYPELIST(IB)
    MC_COMPONENT
    MC_BEANNAME("c")
    Q_PROPERTY(QString text READ text WRITE setText)    //!< 使用getter和setter形式
    MC_AUTOWIRED("r")
    //!< 如果外界并不需要使用对象r，则可以直接使用MEMBER形式。具体请查阅QT官方文档
    Q_PROPERTY(QSharedPointer<R> r MEMBER m_r)
    Q_PROPERTY(QList<QString> texts MEMBER m_texts)
    Q_PROPERTY(QVector<QSharedPointer<R>> rs MEMBER m_rs)
    Q_PROPERTY(StringMap mtexts MEMBER m_mtexts)
    Q_PROPERTY(RHash hrs MEMBER m_hrs)
    Q_PROPERTY(RHash hrs2 MEMBER m_hrs2)
    Q_PROPERTY(RHash hrs3 MEMBER m_hrs3)
    Q_PROPERTY(Qt::AlignmentFlag align MEMBER m_align)
    MC_AUTOWIRED("gadget = gadgetTest")
    Q_PROPERTY(GadgetTestPtr gadget MEMBER m_gadget)
public:
    Q_INVOKABLE C(){}
    
    void a() noexcept override;
    
    QString text() const noexcept;
    void setText(const QString &val) noexcept;
    
    /*!
     * \brief start
     * 
     * 被MC_BEAN_START宏标记的函数将会在C被构造后，属性未被注入前调用，即m_r等所有属性都是默认值
     */
    Q_INVOKABLE
    MC_BEAN_START
    void start() noexcept;
    
    /*!
     * \brief finished
     * 
     * 当所有属性都注入完成后调用
     */
    Q_INVOKABLE
    MC_BEAN_FINISHED
    void finished() noexcept;
    
    /*!
     * \brief threadFinished
     * 
     * 如果调用过本对象的moveToThread函数移动过生存线程，则移动之后调用此函数，否则不调用
     */
    Q_INVOKABLE
    MC_THREAD_FINISHED
    void threadFinished() noexcept;
    
signals:
    void signal_send();
    
private:
    Qt::AlignmentFlag m_align;
    QString m_text;                     //!< 普通字符串
    QSharedPointer<R> m_r;              //!< 对象
    QList<QString> m_texts;             //!< 字符串列表
    QVector<RPtr> m_rs;                 //!< 对象数组
    QMap<QString, QString> m_mtexts;    //!< 字符串映射表
    QHash<QString, RPtr> m_hrs;         //!< 对象哈希表
    QHash<QString, RPtr> m_hrs2;        //!< 对象哈希表
    QHash<QString, RPtr> m_hrs3;        //!< 对象哈希表
    GadgetTestPtr m_gadget;
};
MC_DECL_METATYPE(C);
