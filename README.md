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
  MC_DEFINE_TYPELIST(QObject, MC_DECL_TYPELIST(Insterface)) // 由于QObject宏为QT自带，没有使用过MC_DEFINE_TYPELIST，所以只需要声明QObject足够，但是Insterface中使用了MC_DEFINE_TYPELIST宏，所以这里需要使用MC_DECL_TYPELIST宏额外指定，但是一定要注意MC_DECL_TYPELIST宏中的类型一定是父类类型
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

3. 需要在头文件类定义末尾调用MC_DECL_METATYPE宏，此宏内部会调用MC_DECL_POINTER:
~~~
.h
class Class {

}
MC_DECL_METATYPE(Class)
~~~

4. 在第2点中的MC_REGISTER_COMPONENT即为声明式注入，调用此宏之后IOC容器就能查找并生成该对象。同时在头文件中调用Q_CLASSINFO(MC_BEANNAME, "obj")为该类指定一个beanName，如上面的例子指定的目标beanName为obj，如果不调用该宏，那么系统默认生成一个以类名首字母小写为名的beanName。同时可以调用Q_CLASSINFO(MC_SINGLETON, "false")为该类指定是否为单例生成，如果不调用，默认为单例。此后就可以在另一个注入到系统中的类中获取到该类的实例了:
~~~
Q_PROPERTY(InterfacePtr interface MEMBER m_interface) // 注意这里的类型为Interface后接上Ptr，而不是QSharedPointer<Interface>
~~~
同样的，你可以调用Q_CLASSINFO("interface", "obj")将interface属性指向beanN为obj的bean，如果不调用，那么将直接以属性名interface查找bean，并且需要在后面将USER字段置为true: Q_PROPERTY(InterfacePtr interface MEMBER m_interface USER true)。<br />
同样的，如果你没有调用过QMetaType::unregisterType函数，那么你可以不用调用MC_REGISTER_COMPONENT宏，只需要调用MC_REGISTER_BEAN_FACTORY宏将类注册到元对象中，并使用Q_CLASSINFO(MC_BEANNAME, "obj")宏声明一个beanName，McAnnotationApplicationContext就会在构造时自动查找所有注册到元对象中的类进而进行注入，前提是你从来都没有调用过QMetaType::unregisterType函数或者没有在McAnnotationApplicationContext构造之前调用。

5. 可以在MC_INIT和MC_INIT_END之间使用Mc::Ioc::connect("test", "interface", "signal()", "this", "slot()")为某一个bean连接信号和槽
   - 第一个参数为beanName
   - 第二个参数为信号发出者，指定为this时表示当前类，否则只能指定当前类中的属性
   - 第三个参数为信号
   - 第四个参数为信号接收者，指定为this时表示当前类，否则只能指定当前类中的属性
   - 第五个参数为槽函数
   - 第六个参数为连接方式
   
以上可以参照QObject::connect编写

6. XML注入方式需要编写XML文档。但值得注意的是，如果只想使用XML方式注入，那么上面使用的MC_REGISTER_COMPONENT宏可以换成MC_REGISTER_BEAN_FACTORY宏，同时，不管哪种方式注入，如果使用了容器，都需要使用MC_REGISTER_LIST_CONVERTER或者MC_REGISTER_MAP_CONVERTER注册该容器。但值得注意的是，在XML中的容器标签只有list和map两个标签，但具体的容器类型可以在类中具体定义

7. 同时提供MC_BEAN_START和MC_BEAN_FINISHED两个宏，用于声明两个函数。被MC_BEAN_START声明的函数将在bean被构造完成属性注入之前被调用；被MC_BEAN_FINISHED声明的函数在整个bean被完全构造之后被调用:
~~~
Q_INVOKABLE
MC_BEAN_START
void start() noexcept;

Q_INVOKABLE
MC_BEAN_FINISHED
void end() noexcept;
~~~
如上，start函数将在bean被构造完成，但属性未被注入之前调用，end函数将在整个bean被完全构造之后调用。同时，如果C继承于B，B继承于A，并且三个类中都有被声明过的函数，那么它们都将会被调用，调用顺序为从超基类到子类。A\>B\>C。<br />
同时增加一个当线程被移动后调用函数的tag MC_THREAD_FINISHED，被该tag声明的函数将在当前bean的线程被移动之后被调用。注意: 如果对象的生存线程未被移动过，那么该函数不会被调用，即: 需要在refresh或者getBean时指定一个与生成bean时的线程不同的线程。<br />
但是值得注意的是，由于QT元对象系统的限制，包括Q_INVOKABLE在内的所有tag都必须在返回值类型前面，即下面的用法是错误的
~~~
Q_INVOKABLE
MC_BEAN_FINISHED
virtual void end() noexcept;
~~~
这不会被元对象系统所识别，而需要将virtual调换一下位置，如下所示
~~~
virtual
Q_INVOKABLE
MC_BEAN_FINISHED
void end() noexcept;
~~~
8. 提供一个接口IMcDeleteThreadWhenQuit，当一个实现至该接口的bean构造完成时会调用该接口中的deleteWhenQuit方法(只要实现了该接口就一定会调用)，同时提供一个默认的McDefaultDeleteThreadWhenQuit类，继承该类后，当bean被析构时会同时析构bean的生存线程，注意: 无论线程是否是主线程都会析构，所以如果需要实现复杂功能，需要自己实现IMcDeleteThreadWhenQuit接口。

以上都可在Test代码中找到相应用法用例。

# 注意
- 不能存在一样的bean name，如果存在，则只会保留最后一个。在XML文件中最后面的为最后一个，但是用声明式方法时无法确保先后
- 不要声明一个beanName为this的bean
- 如果bean中没有将要注入的属性，那么容器会调用QObject::setProperty函数注入动态属性
- 受QT自身插件系统性质影响，同一个插件在同一个程序中只能创建一次，即不能将同一个插件声明两次bean：
~~~
<bean plugin="p1"></bean>
<bean plugin="p1"></bean>
~~~
~~~
<bean plugin="p1"></bean>
<list plugins="包含文件p1的路径"></list>
~~~
即上面的用法是错误的，因为QT的插件在同一个程序中是单例的，这会造成多次析构同一个插件。但可以将同一个插件bean多处使用。
- 框架提供返回单个model的方法，但是返回的类型只能为QObject\*或者QAbstractItemModel\*，如果你需要一次性返回多个model，那么可以使用QT内建类型QVariantList或者QVariantMap来实现，即函数的返回值使用这两个中的任意一种。如果使用QVariantList，那么可以当做JS中的Array，如果使用QVariantMap，则可当做JS中的object
- getBean时生成的bean的生存线程问题。由于QT对线程的强控制，所以在getBean之后可能会存在线程冲突问题，所以容器内使用自动和手动指定线程的两种形式:
   1. 如果指定的目标线程为空，那么获取到的bean及其QObject属性都将生存在调用getBean时的线程中。注意: 
      - 这里的getBean指的是IMcBeanFactory的getBean，而不是IMcApplicationContext。因为IMcApplicationContext存在refresh函数自动加载bean，此时bean的生存线程将是调用refresh时的线程，如果不调用refresh，那么bean的生存线程才是getBean时的线程
      - 有如下形式: 
         1. 如果在次线程中创建appContext，如果在次线程中调用getBean，那么获取到的bean的线程为次线程。如果在主线程调用getBean，那么获取到的bean的线程为主线程。
         2. 如果在次线程中创建appContext，如果在次线程中调用了refresh函数，那么getBean获取到的bean的线程永远在次线程，反之，如果在主线程中调用refresh，那么bean的线程在主线程。
         3. 上面两种情况只适用于将isSingleton设置为true时的情况，如果isSingleton为false，那么每次调用getBean时都会创建一个新对象，此时获取到的bean就永远为调用getBean时的线程。
   2. 可以在调用getBean或refresh函数时指定一个目标线程，那么目标bean的生存线程将为指定的线程。同样的，只适用于isSingleton为true的情况。同时，如果isSingleton指定为true，但是在次线程中调用了refresh函数，就算在主线程中调用getBean时指定了目标线程也不会起作用，因为只能在对象创建时才能指定线程。
   3. 提供上面形式的线程指定方式的主要目的在于，如果XML中声明的对象结构过于复杂，对象的创建可能耗时太长，那么可以在子线程中完成创建，并且能指定目标线程。
   
   
# IocBoot
- 提供IocBoot和其静态函数run，以提供一个默认的QML到C++交互的方式：
   1. 在QML中提供一个名为$的请求器，并提供invoke函数的两种重载函数来请求Controller，invoke函数为C++函数，不可重写
   2. 在$中又提供了get和post两个函数以此封装invoke函数，这两个函数时js函数，可以重新赋值
   3. 以上函数的请求方式均为异步请求
   4. 以上函数的返回值为Response，可以调用then方法来获取请求结果。then方法接收一个js function，并通过一个参数表现其返回值。同时，由于QML自身性质限制，部分操作不能在其它线程，所以提供了一个同步版本的syncThen函数来让线程所有权回到主线程时再调用回调函数
   5. 通过以上方法请求Controller时可以传递包括QObject\*在内的任何QT元对象能获取到的类型。QObject\*对应js中的object。必须使用QSharedPointer\<QObject\>。同时返回值亦为所有类型，但建议只是用QString、QJsonObject、QObject*
   6. 请求方式为
   ~~~
   $.get("beanName.funcName?param1=1&param2=2")
   ~~~
   或者
   ~~~
   $.post("beanName.funcName", {
       param1: 1,
       param2: "2",
       param3: {
           param1: 1
       }
   })
   ~~~
   上面beanName为mcRegisterBeanFactory注册时的参数，funcName为Controller的函数，param1、param2、param3为函数的参数名，其后的值为将要赋值的参数值，其中param3将会被构造成一个QSharedPointer\<QObject\>
   
   7. Controller需要使用Q_CLASSINFO(MC_COMPONENT, MC_CONTROLLER)来声明
   8. 注意: 无论是$.get还是$.post亦或者是$.invoke的返回值都不允许赋值给其他变量，因为它们的返回值将在整个函数调用完毕时被析构。
- 增加QML到C++的长连接QmlSocket通信方式：
   1. C++端声明一个Component，并使用Q_CLASSINFO(MC_COMPONENT, MC_QML_SOCKET)附加额外属性。然后按照需求实现最多四个函数，并使用四种宏标志四个函数以接收各种消息，具体参照QmlSocketTest或者Java Spring WebSocket。注意：每一个函数的执行都是在另外的线程。
   2. QML端可以使用$.qs("beanName")来发起一个请求，该函数会返回一个对象，参照JS WebSocket。同时$.qs函数拥有第二个参数，可以直接指定onOpen等回调函数。同时因为部分界面操作不能在其他线程执行，所以$.qs的第二个参数中可以指定isOpenSync等参数来让某一个回调函数回到主线程后再执行。具体参照main.qml。
   3. 注意: $.qs的返回值必须赋值给某一特定变量，否则它可能会被QJSEngine的垃圾回收器给回收掉。
## 2020-6-3重大注意事项
$的所有请求(get/post/qs)全部需要使用syncThen方式，否则会出现抢资源的而导致不可预料的崩溃。
   
   
