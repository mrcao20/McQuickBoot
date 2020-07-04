INCLUDEPATH += $$PWD/include/

HEADERS += \
    $$PWD/include/McIoc/ApplicationContext/IMcApplicationContext.h \
    $$PWD/include/McIoc/ApplicationContext/IMcReadableApplicationContext.h \
    $$PWD/include/McIoc/ApplicationContext/IMcRefreshableApplicationContext.h \
    $$PWD/include/McIoc/ApplicationContext/McContainerGlobal.h \
    $$PWD/include/McIoc/ApplicationContext/impl/McAbstractApplicationContext.h \
    $$PWD/include/McIoc/ApplicationContext/impl/McAnnotationApplicationContext.h \
    $$PWD/include/McIoc/ApplicationContext/impl/McLocalPathApplicationContext.h \
    $$PWD/include/McIoc/ApplicationContext/impl/McReadableApplicationContext.h \
    $$PWD/include/McIoc/ApplicationContext/impl/McXmlApplicationContext.h \
    $$PWD/include/McIoc/BeanDefinition/IMcBeanDefinition.h \
    $$PWD/include/McIoc/BeanDefinition/impl/McRootBeanDefinition.h \
    $$PWD/include/McIoc/BeanDefinitionReader/IMcBeanDefinitionReader.h \
    $$PWD/include/McIoc/BeanDefinitionReader/impl/McAbstractBeanDefinitionReader.h \
    $$PWD/include/McIoc/BeanDefinitionReader/impl/McAnnotationBeanDefinitionReader.h \
    $$PWD/include/McIoc/BeanDefinitionReader/impl/McXmlBeanDefinitionReader.h \
    $$PWD/include/McIoc/BeanFactory/IMcBeanDefinitionRegistry.h \
    $$PWD/include/McIoc/BeanFactory/IMcBeanFactory.h \
    $$PWD/include/McIoc/BeanFactory/IMcBeanReferenceResolver.h \
    $$PWD/include/McIoc/BeanFactory/IMcConfigurableBeanFactory.h \
    $$PWD/include/McIoc/BeanFactory/McBeanGlobal.h \
    $$PWD/include/McIoc/BeanFactory/impl/McAbstractBeanFactory.h \
    $$PWD/include/McIoc/BeanFactory/impl/McBeanConnector.h \
    $$PWD/include/McIoc/BeanFactory/impl/McBeanEnum.h \
    $$PWD/include/McIoc/BeanFactory/impl/McBeanReference.h \
    $$PWD/include/McIoc/BeanFactory/impl/McDefaultBeanFactory.h \
    $$PWD/include/McIoc/BeanFactory/impl/McMetaTypeId.h \
    $$PWD/include/McIoc/PropertyParser/IMcPropertyConverter.h \
    $$PWD/include/McIoc/PropertyParser/IMcPropertyParser.h \
    $$PWD/include/McIoc/PropertyParser/impl/McAbstarctPropertyConverter.h \
    $$PWD/include/McIoc/PropertyParser/impl/McAbstractPropertyParser.h \
    $$PWD/include/McIoc/PropertyParser/impl/McDefaultPropertyConverter.h \
    $$PWD/include/McIoc/PropertyParser/impl/McDefaultPropertyParser.h \
    $$PWD/include/McIoc/Thread/IMcDeleteThreadWhenQuit.h \
    $$PWD/include/McIoc/Thread/impl/McDefaultDeleteThreadWhenQuit.h \
    $$PWD/include/McIoc/Utils/McScopedFunction.h \
    include/McIoc/McGlobal.h \
    include/McIoc/McIocVersion.h \
    include/McIoc/McMacroGlobal.h

SOURCES += \
    $$PWD/src/ApplicationContext/McAbstractApplicationContext.cpp \
    $$PWD/src/ApplicationContext/McAnnotationApplicationContext.cpp \
    $$PWD/src/ApplicationContext/McContainerGlobal.cpp \
    $$PWD/src/ApplicationContext/McLocalPathApplicationContext.cpp \
    $$PWD/src/ApplicationContext/McReadableApplicationContext.cpp \
    $$PWD/src/ApplicationContext/McXmlApplicationContext.cpp \
    $$PWD/src/BeanDefinitionReader/McAbstractBeanDefinitionReader.cpp \
    $$PWD/src/BeanDefinitionReader/McAnnotationBeanDefinitionReader.cpp \
    $$PWD/src/BeanDefinitionReader/McXmlBeanDefinitionReader.cpp \
    $$PWD/src/BeanFactory/McAbstractBeanFactory.cpp \
    $$PWD/src/BeanFactory/McBeanConnector.cpp \
    $$PWD/src/BeanFactory/McBeanEnum.cpp \
    $$PWD/src/BeanFactory/McBeanReference.cpp \
    $$PWD/src/BeanFactory/McDefaultBeanFactory.cpp \
    $$PWD/src/BeanFactory/McMetaTypeId.cpp \
    $$PWD/src/PropertyParser/McAbstarctPropertyConverter.cpp \
    $$PWD/src/PropertyParser/McAbstractPropertyParser.cpp \
    $$PWD/src/PropertyParser/McDefaultPropertyConverter.cpp \
    $$PWD/src/PropertyParser/McDefaultPropertyParser.cpp \
    $$PWD/src/Thread/McDefaultDeleteThreadWhenQuit.cpp \
    src/McGlobal.cpp
