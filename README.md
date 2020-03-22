# McIocBoot
基于QT元对象系统封装出来的以IOC容器为核心的框架

# Usage
目前Ioc Container部分包含声明式注入和XML文件注入两种方式，但无论是那种注入方式都需要使用QSharedPointer动态指针的形式，并且需要使用MC_DECL_POINTER宏将QSharedPointer\<Class\>重新定义为ClassPtr的形式使用，否则无法识别。
1. 这里建议所有自定义类型都使用MC_DEFINE_TYPELIST宏定义父类:
~~~
Insterface.h
class Insterface {
  MC_DEFINE_TYPELIST()  // 这里定义为空，因为本类没有父类
}
-------------------
class Object : public QObject, public Insterface{
  MC_DEFINE_TYPELIST(QObject, MC_DECL_TYPELIST(Insterface)) // 由于QObject宏为QT自带，没有使用过MC_DEFINE_TYPELIST，所以只需要声明QObject足够，但是Insterface中使用了MC_DEFINE_TYPELIST宏，所以这里需要使用MC_DECL_TYPELIST宏额外指定
}
~~~
以上主要用于定义类之间的层级关系

2. 使用MC_DECL_INIT、MC_INIT和MC_INIT_END三个宏在类中向QT元对象系统注册所需数据: 
~~~
.h
MC_DECL_INIT(Class)
.cpp
MC_INIT(Class)
MC_REGISTER_COMPONENT(MC_TYPELIST(Class))
MC_INIT_END
~~~
如上，可以在MC_INIT和MC_INIT_END之间编写程序运行之前的额外代码，通常用于注册元对象

3. 需要在头文件类定义末尾调用MC_DECL_POINTER和MC_DECL_METATYPE宏:
~~~
.h
class Class {

}
MC_DECL_POINTER(Class)
MC_DECL_METATYPE(Class)
~~~

4. 在第2点中的MC_REGISTER_COMPONENT即为声明式注入，调用此宏之后IOC容器就能查找并生成该对象。同时在头文件中调用Q_CLASSINFO(MC_BEANNAME, "obj")为该类指定一个beanName，如上面的例子指定的目标beanName为obj，如果不调用该宏，那么系统默认生成一个以类名首字母小写为名的beanName。同时可以调用Q_CLASSINFO(MC_SINGLETON, "false")为该类指定是否为单例生成，如果不调用，默认为单例。此后就可以在另一个注入到系统中的类中获取到该类的实例了:
~~~
Q_PROPERTY(InterfacePtr interface MEMBER m_interface) // 注意这里的类型为Interface后接上Ptr，而不是QSharedPointer<Interface>
~~~
同样的，你可以调用Q_CLASSINFO("interface", "obj")将interface属性指向beanN为obj的bean，如果不调用，那么将直接以属性名interface查找bean，并且需要在后面将USER字段置为true: Q_PROPERTY(InterfacePtr interface MEMBER m_interface USER true)

5. 可以在MC_INIT和MC_INIT_END之间使用Mc::Ioc::connect("test", "interface", "signal()", "this", "slot()")为某一个bean连接信号和槽
   - 第一个参数为beanName
   - 第二个参数为信号发出者，指定为this时表示当前类，否则只能指定当前类中的属性
   - 第三个参数为信号
   - 第四个参数为信号接收者，指定为this时表示当前类，否则只能指定当前类中的属性
   - 第五个参数为槽函数
   - 第六个参数为连接方式
   
以上可以参照QObject::connect编写

6. XML注入方式需要编写XML文档。但值得注意的是，如果只想使用XML方式注入，那么上面使用的MC_REGISTER_COMPONENT宏可以换成MC_REGISTER_BEAN_FACTORY宏，同时，不管哪种方式注入，如果使用了容器，都需要使用MC_REGISTER_LIST_CONVERTER或者MC_REGISTER_MAP_CONVERTER注册该容器

以上都可在Test代码中找到相应用法用例。

