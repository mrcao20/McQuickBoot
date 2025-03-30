INCLUDEPATH += $$PWD

contains(DEFINES, MC_USE_LIBRARY) {
    DEFINES += MC_IOC_LIBRARY
}

HEADERS += \
    $$PWD/McIocMacroGlobal.h \
    $$PWD/McIocConstantGlobal.h \
    $$PWD/McIocGlobal.h \
    $$PWD/Utils/IMcBeanBuildable.h \
    $$PWD/Utils/XmlBuilder/IMcXml.h \
    $$PWD/Utils/XmlBuilder/IMcBeanContent.h \
    $$PWD/Utils/XmlBuilder/IMcPropertyContent.h \
    $$PWD/Utils/XmlBuilder/IMcBean.h \
    $$PWD/Utils/XmlBuilder/Impl/McBeanCollection.h \
    $$PWD/Utils/XmlBuilder/Impl/McBean.h \
    $$PWD/Utils/XmlBuilder/Impl/McClassBean.h \
    $$PWD/Utils/XmlBuilder/Impl/McPluginBean.h \
    $$PWD/Utils/XmlBuilder/Impl/McConnect.h \
    $$PWD/Utils/XmlBuilder/Impl/McEnum.h \
    $$PWD/Utils/XmlBuilder/Impl/McList.h \
    $$PWD/Utils/XmlBuilder/Impl/McMap.h \
    $$PWD/Utils/XmlBuilder/Impl/McPlaceholder.h \
    $$PWD/Utils/XmlBuilder/Impl/McProperty.h \
    $$PWD/Utils/XmlBuilder/Impl/McRef.h \
    $$PWD/Utils/XmlBuilder/Impl/McValue.h \
    $$PWD/Utils/XmlBuilder/Impl/McConstructorArg.h \
    $$PWD/Utils/XmlBuilder/Impl/McPluginChecker.h \
    $$PWD/BeanFactory/IMcBeanFactory.h \
    $$PWD/BeanFactory/IMcBeanBuilderRegistry.h \
    $$PWD/BeanFactory/IMcConfigurableBeanFactory.h \
    $$PWD/BeanFactory/Impl/McAbstractBeanFactory.h \
    $$PWD/BeanFactory/Impl/McDefaultBeanFactory.h \
    $$PWD/BeanFactory/Impl/McResolverBeanFactory.h \
    $$PWD/BeanBuilder/IMcBeanBuilder.h \
    $$PWD/BeanBuilder/IMcBeanReferenceResolver.h \
    $$PWD/BeanBuilder/IMcCustomPlaceholder.h \
    $$PWD/BeanBuilder/IMcCustomBeanBuilderFactory.h \
    $$PWD/BeanBuilder/McCustomBeanBuilderContainer.h \
    $$PWD/BeanBuilder/Impl/McBeanReference.h \
    $$PWD/BeanBuilder/Impl/McBeanPlaceholder.h \
    $$PWD/BeanBuilder/Impl/McBeanEnum.h \
    $$PWD/BeanBuilder/Impl/McBeanConnector.h \
    $$PWD/BeanBuilder/Impl/McAbstractBeanBuilder.h \
    $$PWD/BeanBuilder/Impl/McObjectBeanBuilder.h \
    $$PWD/BeanBuilder/Impl/McObjectClassBeanBuilder.h \
    $$PWD/BeanBuilder/Impl/McSharedObjectClassBeanBuilder.h \
    $$PWD/BeanBuilder/Impl/McObjectPluginBeanBuilder.h \
    $$PWD/BeanBuilder/Impl/McSharedObjectPluginBeanBuilder.h \
    $$PWD/BeanBuilder/Impl/McGadgetBeanBuilder.h \
    $$PWD/BeanBuilder/Impl/McSharedGadgetBeanBuilder.h \
    $$PWD/BeanBuilder/Impl/McListBeanBuilder.h \
    $$PWD/BeanBuilder/Impl/McMapBeanBuilder.h \
    $$PWD/BeanBuilder/Impl/McSimpleBeanBuilder.h \
    $$PWD/BeanBuilder/Impl/McCustomBeanBuilder.h \
    $$PWD/BeanBuilder/Impl/McCustomBeanBuilderFactory.h \
    $$PWD/BeanBuilderReader/IMcBeanBuilderReader.h \
    $$PWD/BeanBuilderReader/Impl/McAbstractBeanBuilderReader.h \
    $$PWD/BeanBuilderReader/Impl/McXmlBeanBuilderReader.h \
    $$PWD/BeanBuilderReader/Impl/McAnnotationBeanDefinitionReader.h \
    $$PWD/ApplicationContext/IMcRefreshableApplicationContext.h \
    $$PWD/ApplicationContext/IMcApplicationContext.h \
    $$PWD/ApplicationContext/IMcReadableApplicationContext.h \
    $$PWD/ApplicationContext/Impl/McAbstractApplicationContext.h \
    $$PWD/ApplicationContext/Impl/McReadableApplicationContext.h \
    $$PWD/ApplicationContext/Impl/McXmlApplicationContext.h \
    $$PWD/ApplicationContext/Impl/McLocalPathApplicationContext.h \
    $$PWD/ApplicationContext/Impl/McAnnotationApplicationContext.h

SOURCES += \
    $$PWD/McIocGlobal.cpp \
    $$PWD/Utils/XmlBuilder/Impl/McBeanCollection.cpp \
    $$PWD/Utils/XmlBuilder/Impl/McBean.cpp \
    $$PWD/Utils/XmlBuilder/Impl/McClassBean.cpp \
    $$PWD/Utils/XmlBuilder/Impl/McPluginBean.cpp \
    $$PWD/Utils/XmlBuilder/Impl/McConnect.cpp \
    $$PWD/Utils/XmlBuilder/Impl/McEnum.cpp \
    $$PWD/Utils/XmlBuilder/Impl/McList.cpp \
    $$PWD/Utils/XmlBuilder/Impl/McMap.cpp \
    $$PWD/Utils/XmlBuilder/Impl/McPlaceholder.cpp \
    $$PWD/Utils/XmlBuilder/Impl/McProperty.cpp \
    $$PWD/Utils/XmlBuilder/Impl/McRef.cpp \
    $$PWD/Utils/XmlBuilder/Impl/McValue.cpp \
    $$PWD/Utils/XmlBuilder/Impl/McConstructorArg.cpp \
    $$PWD/Utils/XmlBuilder/Impl/McPluginChecker.cpp \
    $$PWD/BeanFactory/Impl/McAbstractBeanFactory.cpp \
    $$PWD/BeanFactory/Impl/McDefaultBeanFactory.cpp \
    $$PWD/BeanFactory/Impl/McResolverBeanFactory.cpp \
    $$PWD/BeanBuilder/McCustomBeanBuilderContainer.cpp \
    $$PWD/BeanBuilder/Impl/McAbstractBeanBuilder.cpp \
    $$PWD/BeanBuilder/Impl/McObjectBeanBuilder.cpp \
    $$PWD/BeanBuilder/Impl/McObjectClassBeanBuilder.cpp \
    $$PWD/BeanBuilder/Impl/McSharedObjectClassBeanBuilder.cpp \
    $$PWD/BeanBuilder/Impl/McObjectPluginBeanBuilder.cpp \
    $$PWD/BeanBuilder/Impl/McSharedObjectPluginBeanBuilder.cpp \
    $$PWD/BeanBuilder/Impl/McGadgetBeanBuilder.cpp \
    $$PWD/BeanBuilder/Impl/McSharedGadgetBeanBuilder.cpp \
    $$PWD/BeanBuilder/Impl/McListBeanBuilder.cpp \
    $$PWD/BeanBuilder/Impl/McMapBeanBuilder.cpp \
    $$PWD/BeanBuilder/Impl/McSimpleBeanBuilder.cpp \
    $$PWD/BeanBuilderReader/Impl/McAbstractBeanBuilderReader.cpp \
    $$PWD/BeanBuilderReader/Impl/McXmlBeanBuilderReader.cpp \
    $$PWD/BeanBuilderReader/Impl/McAnnotationBeanDefinitionReader.cpp \
    $$PWD/ApplicationContext/Impl/McAbstractApplicationContext.cpp \
    $$PWD/ApplicationContext/Impl/McReadableApplicationContext.cpp \
    $$PWD/ApplicationContext/Impl/McXmlApplicationContext.cpp \
    $$PWD/ApplicationContext/Impl/McLocalPathApplicationContext.cpp \
    $$PWD/ApplicationContext/Impl/McAnnotationApplicationContext.cpp
