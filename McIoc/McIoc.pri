QT *= xml
QT *= core-private

include($$PWD/../3rdparty/yaml-cpp.pri)
include($$PWD/../3rdparty/zlib.pri)
include($$PWD/../3rdparty/quazip.pri)

INCLUDEPATH += $$PWD/include/

HEADERS += \
    $$PWD/include/McIoc/ApplicationContext/IMcApplicationContext.h \
    $$PWD/include/McIoc/ApplicationContext/IMcReadableApplicationContext.h \
    $$PWD/include/McIoc/ApplicationContext/IMcRefreshableApplicationContext.h \
    $$PWD/include/McIoc/ApplicationContext/impl/McAbstractApplicationContext.h \
    $$PWD/include/McIoc/ApplicationContext/impl/McAnnotationApplicationContext.h \
    $$PWD/include/McIoc/ApplicationContext/impl/McLocalPathApplicationContext.h \
    $$PWD/include/McIoc/ApplicationContext/impl/McReadableApplicationContext.h \
    $$PWD/include/McIoc/ApplicationContext/impl/McXmlApplicationContext.h \
    $$PWD/include/McIoc/BeanDefinition/IMcBeanDefinition.h \
    $$PWD/include/McIoc/BeanDefinition/IMcCustomPlaceholder.h \
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
    $$PWD/include/McIoc/BeanFactory/impl/McAbstractNormalBeanFactory.h \
    $$PWD/include/McIoc/BeanFactory/impl/McBeanConnector.h \
    $$PWD/include/McIoc/BeanFactory/impl/McBeanEnum.h \
    $$PWD/include/McIoc/BeanFactory/impl/McBeanPlaceholder.h \
    $$PWD/include/McIoc/BeanFactory/impl/McBeanReference.h \
    $$PWD/include/McIoc/BeanFactory/impl/McMetaTypeId.h \
    $$PWD/include/McIoc/BeanFactory/impl/McPointerBeanFactory.h \
    $$PWD/include/McIoc/BeanFactory/impl/McSharedBeanFactory.h \
    $$PWD/include/McIoc/Destroyer/IMcDestroyer.h \
    $$PWD/include/McIoc/Destroyer/impl/McNormalDestroyer.h \
    $$PWD/include/McIoc/McConstantGlobal.h \
    $$PWD/include/McIoc/McVersion.h \
    $$PWD/include/McIoc/PropertyParser/IMcPropertyConverter.h \
    $$PWD/include/McIoc/PropertyParser/IMcPropertyParser.h \
    $$PWD/include/McIoc/PropertyParser/impl/McAbstarctPropertyConverter.h \
    $$PWD/include/McIoc/PropertyParser/impl/McAbstractPropertyParser.h \
    $$PWD/include/McIoc/PropertyParser/impl/McDefaultPropertyConverter.h \
    $$PWD/include/McIoc/PropertyParser/impl/McDefaultPropertyParser.h \
    $$PWD/include/McIoc/Signal/IMcSignal.h \
    $$PWD/include/McIoc/Slot/IMcSlot.h \
    $$PWD/include/McIoc/Thread/IMcDeleteThreadWhenQuit.h \
    $$PWD/include/McIoc/Thread/impl/McDefaultDeleteThreadWhenQuit.h \
    $$PWD/include/McIoc/Utils/IMcNonCopyable.h \
    $$PWD/include/McIoc/Utils/Macro/MacroFillingFunc.h \
    $$PWD/include/McIoc/Utils/Macro/MacroSize.h \
    $$PWD/include/McIoc/Utils/McScopedFunction.h \
    $$PWD/include/McIoc/McGlobal.h \
    $$PWD/include/McIoc/McMacroGlobal.h \
    $$PWD/include/McIoc/Utils/XmlBuilder/impl/McBean.h \
    $$PWD/include/McIoc/Utils/XmlBuilder/impl/McBeanCollection.h \
    $$PWD/include/McIoc/Utils/XmlBuilder/impl/McPlaceholder.h \
    $$PWD/include/McIoc/Utils/XmlBuilder/impl/McProperty.h \
    $$PWD/include/McIoc/Utils/XmlBuilder/IMcXml.h \
    $$PWD/include/McIoc/Utils/XmlBuilder/IMcBean.h \
    $$PWD/include/McIoc/Utils/XmlBuilder/IMcBeanContent.h \
    $$PWD/include/McIoc/Utils/XmlBuilder/IMcPropertyContent.h \
    $$PWD/include/McIoc/Utils/XmlBuilder/impl/McConnect.h \
    $$PWD/include/McIoc/Utils/XmlBuilder/impl/McEnum.h \
    $$PWD/include/McIoc/Utils/XmlBuilder/impl/McValue.h \
    $$PWD/include/McIoc/Utils/XmlBuilder/impl/McMap.h \
    $$PWD/include/McIoc/Utils/XmlBuilder/impl/McList.h \
    $$PWD/include/McIoc/Utils/XmlBuilder/impl/McRef.h \
    $$PWD/include/McIoc/BeanDefinitionReader/impl/McSettingBeanDefinitionReader.h \
    $$PWD/include/McIoc/ApplicationContext/impl/McSettingApplicationContext.h \
    $$PWD/include/McIoc/ApplicationContext/impl/McIniSettingApplicationContext.h \
    $$PWD/include/McIoc/ApplicationContext/IMcRelatableApplicationContext.h \
    $$PWD/include/McIoc/ApplicationContext/IMcConfigurableApplicationContext.h \
    $$PWD/include/McIoc/Utils/McYaml.h \
    $$PWD/include/McIoc/ApplicationContext/impl/McYamlSettingApplicationContext.h \
    $$PWD/include/McIoc/Utils/Zip/McCompressor.h

SOURCES += \
    $$PWD/src/ApplicationContext/McAbstractApplicationContext.cpp \
    $$PWD/src/ApplicationContext/McAnnotationApplicationContext.cpp \
    $$PWD/src/ApplicationContext/McLocalPathApplicationContext.cpp \
    $$PWD/src/ApplicationContext/McReadableApplicationContext.cpp \
    $$PWD/src/ApplicationContext/McXmlApplicationContext.cpp \
    $$PWD/src/BeanDefinitionReader/McAbstractBeanDefinitionReader.cpp \
    $$PWD/src/BeanDefinitionReader/McAnnotationBeanDefinitionReader.cpp \
    $$PWD/src/BeanDefinitionReader/McXmlBeanDefinitionReader.cpp \
    $$PWD/src/BeanFactory/McAbstractBeanFactory.cpp \
    $$PWD/src/BeanFactory/McAbstractNormalBeanFactory.cpp \
    $$PWD/src/BeanFactory/McMetaTypeId.cpp \
    $$PWD/src/BeanFactory/McPointerBeanFactory.cpp \
    $$PWD/src/BeanFactory/McSharedBeanFactory.cpp \
    $$PWD/src/McInitGlobal.cpp \
    $$PWD/src/PropertyParser/McAbstarctPropertyConverter.cpp \
    $$PWD/src/PropertyParser/McAbstractPropertyParser.cpp \
    $$PWD/src/PropertyParser/McDefaultPropertyConverter.cpp \
    $$PWD/src/PropertyParser/McDefaultPropertyParser.cpp \
    $$PWD/src/Thread/McDefaultDeleteThreadWhenQuit.cpp \
    $$PWD/src/McGlobal.cpp \
    $$PWD/src/Utils/XmlBuilder/McBeanCollection.cpp \
    $$PWD/src/Utils/XmlBuilder/McBean.cpp \
    $$PWD/src/Utils/XmlBuilder/McPlaceholder.cpp \
    $$PWD/src/Utils/XmlBuilder/McProperty.cpp \
    $$PWD/src/Utils/XmlBuilder/McConnect.cpp \
    $$PWD/src/Utils/XmlBuilder/McEnum.cpp \
    $$PWD/src/Utils/XmlBuilder/McList.cpp \
    $$PWD/src/Utils/XmlBuilder/McMap.cpp \
    $$PWD/src/Utils/XmlBuilder/McRef.cpp \
    $$PWD/src/Utils/XmlBuilder/McValue.cpp \
    $$PWD/src/BeanDefinitionReader/McSettingBeanDefinitionReader.cpp \
    $$PWD/src/ApplicationContext/McSettingApplicationContext.cpp \
    $$PWD/src/ApplicationContext/McIniSettingApplicationContext.cpp \
    $$PWD/src/Utils/McYaml.cpp \
    $$PWD/src/ApplicationContext/McYamlSettingApplicationContext.cpp \
    $$PWD/src/Utils/Zip/McCompressor.cpp
