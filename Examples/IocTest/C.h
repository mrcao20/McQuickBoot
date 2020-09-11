#pragma once
#include <McIoc/McGlobal.h>
class R : public QObject
{
    Q_OBJECT
    MC_DECL_INIT(R)
    Q_CLASSINFO(MC_BEANNAME, "r")
    Q_PROPERTY(QString text READ text WRITE setText);
public:
    Q_INVOKABLE R(){}
    
    QString text() const noexcept;
    void setText(const QString &val) noexcept;
    
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
MC_DECL_METATYPE(IA);                   //!< 这里必须使用该宏声明，否则无法从C转换到该接口。

class IB : public IA
{
    MC_DEFINE_TYPELIST(IA);             //!< 由于本接口有一个父接口，并且可能存在从IB转换到IA，所以这里需要使用这个宏保存父接口
public:
};
MC_DECL_METATYPE(IB);

class C : public QObject, public IB
{
    Q_OBJECT
    MC_DECL_INIT(C)                     //!< 这个宏主要用来实现一个类似于java静态代码块的功能。这里只是声明，真正实现在cpp中
    //! 同理，由于C实现至IB接口，并且可能转换到IB，所以这里需要使用该宏。
    //! 这里需要使用MC_DECL_TYPELIST宏的原因在于IB继承了其他父接口，并且C也可能转换到IB的其他父接口，所以需要使用该宏额外标识。注意：IB必须使用过MC_DEFINE_TYPELIST后才能使用该宏
    //! 这里不需要额外指定QOBject，容器会自动指定。但如果C继承至其他类，比如QWidget，那么需要先使用MC_DECL_METATYPE声明QWidget，再使用MC_DEFINE_TYPELIST(QWidget, MC_DECL_TYPELIST(IB))，
    //! 当然，如果不需要从C转换到QWidget，也就不需要额外声明QWidget
    MC_DEFINE_TYPELIST(MC_DECL_TYPELIST(IB))
    Q_CLASSINFO(MC_BEANNAME, "c")
    Q_PROPERTY(QString text READ text WRITE setText)    //!< 使用getter和setter形式
    Q_PROPERTY(RPtr r MEMBER m_r USER true)             //!< 如果外界并不需要使用对象r，则可以直接使用MEMBER形式。具体请查阅QT官方文档
    //!< r的注入的两种形式，这里因为属性r和beanNamer同名，所以可以直接添加USER true，否则需要使用下面这种形式
    //!< Q_CLASSINFO("r", "r")
    Q_PROPERTY(QList<QString> texts MEMBER m_texts)
    Q_PROPERTY(QVector<RPtr> rs MEMBER m_rs)
    typedef QMap<QString, QString> StringMap;           //!< 由于QMap在Q_PROPERTY宏中有错误提示，所以这里先重定义一下
    Q_PROPERTY(StringMap mtexts MEMBER m_mtexts)
    typedef QHash<QString, RPtr> RHash; 
    Q_PROPERTY(RHash hrs MEMBER m_hrs)
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
    QString m_text;                     //!< 普通字符串
    RPtr m_r;                           //!< 对象
    QList<QString> m_texts;             //!< 字符串列表
    QVector<RPtr> m_rs;                 //!< 对象数组
    QMap<QString, QString> m_mtexts;    //!< 字符串映射表
    QHash<QString, RPtr> m_hrs;         //!< 对象哈希表
};
MC_DECL_METATYPE(C);
