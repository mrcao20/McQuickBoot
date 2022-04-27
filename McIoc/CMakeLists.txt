project(McIoc)

include_directories(${CMAKE_SOURCE_DIR})
include_directories(${CMAKE_BINARY_DIR}/McCore)

set(PROJECT_HEADERS
    McIocMacroGlobal.h
    McIocConstantGlobal.h
    McIocGlobal.h
    Utils/IMcBeanBuildable.h
    BeanFactory/IMcBeanFactory.h
    BeanFactory/IMcBeanBuilderRegistry.h
    BeanFactory/IMcConfigurableBeanFactory.h
    BeanFactory/Impl/McAbstractBeanFactory.h
    BeanFactory/Impl/McDefaultBeanFactory.h
    BeanFactory/Impl/McResolverBeanFactory.h
    BeanBuilder/IMcBeanBuilder.h
    BeanBuilder/IMcBeanReferenceResolver.h
    BeanBuilder/IMcCustomPlaceholder.h
    BeanBuilder/Impl/McBeanReference.h
    BeanBuilder/Impl/McBeanPlaceholder.h
    BeanBuilder/Impl/McBeanEnum.h
    BeanBuilder/Impl/McBeanConnector.h
    BeanBuilder/Impl/McAbstractBeanBuilder.h
    BeanBuilder/Impl/McObjectBeanBuilder.h
    BeanBuilder/Impl/McObjectClassBeanBuilder.h
    BeanBuilder/Impl/McSharedObjectClassBeanBuilder.h
    BeanBuilder/Impl/McObjectPluginBeanBuilder.h
    BeanBuilder/Impl/McSharedObjectPluginBeanBuilder.h
    BeanBuilder/Impl/McGadgetBeanBuilder.h
    BeanBuilder/Impl/McSharedGadgetBeanBuilder.h
    BeanBuilder/Impl/McListBeanBuilder.h
    BeanBuilder/Impl/McMapBeanBuilder.h
    BeanBuilder/Impl/McSimpleBeanBuilder.h
    BeanBuilderReader/IMcBeanBuilderReader.h
    BeanBuilderReader/Impl/McAbstractBeanBuilderReader.h
    BeanBuilderReader/Impl/McXmlBeanBuilderReader.h
    BeanBuilderReader/Impl/McAnnotationBeanDefinitionReader.h
    ApplicationContext/IMcRefreshableApplicationContext.h
    ApplicationContext/IMcApplicationContext.h
    ApplicationContext/IMcReadableApplicationContext.h
    ApplicationContext/Impl/McAbstractApplicationContext.h
    ApplicationContext/Impl/McReadableApplicationContext.h
    ApplicationContext/Impl/McXmlApplicationContext.h
    ApplicationContext/Impl/McLocalPathApplicationContext.h
    ApplicationContext/Impl/McAnnotationApplicationContext.h
)

set(PROJECT_SOURCES
    McIocGlobal.cpp
    BeanFactory/Impl/McAbstractBeanFactory.cpp
    BeanFactory/Impl/McDefaultBeanFactory.cpp
    BeanFactory/Impl/McResolverBeanFactory.cpp
    BeanBuilder/Impl/McAbstractBeanBuilder.cpp
    BeanBuilder/Impl/McObjectBeanBuilder.cpp
    BeanBuilder/Impl/McObjectClassBeanBuilder.cpp
    BeanBuilder/Impl/McSharedObjectClassBeanBuilder.cpp
    BeanBuilder/Impl/McObjectPluginBeanBuilder.cpp
    BeanBuilder/Impl/McSharedObjectPluginBeanBuilder.cpp
    BeanBuilder/Impl/McGadgetBeanBuilder.cpp
    BeanBuilder/Impl/McSharedGadgetBeanBuilder.cpp
    BeanBuilder/Impl/McListBeanBuilder.cpp
    BeanBuilder/Impl/McMapBeanBuilder.cpp
    BeanBuilder/Impl/McSimpleBeanBuilder.cpp
    BeanBuilderReader/Impl/McAbstractBeanBuilderReader.cpp
    BeanBuilderReader/Impl/McXmlBeanBuilderReader.cpp
    BeanBuilderReader/Impl/McAnnotationBeanDefinitionReader.cpp
    ApplicationContext/Impl/McAbstractApplicationContext.cpp
    ApplicationContext/Impl/McReadableApplicationContext.cpp
    ApplicationContext/Impl/McXmlApplicationContext.cpp
    ApplicationContext/Impl/McLocalPathApplicationContext.cpp
    ApplicationContext/Impl/McAnnotationApplicationContext.cpp
)

add_library(${PROJECT_NAME} ${MC_BUILD_TYPE}
    ${PROJECT_HEADERS}
    ${PROJECT_SOURCES}
)

target_link_libraries(${PROJECT_NAME} PRIVATE Qt6::Core
    McCore)

target_compile_definitions(${PROJECT_NAME} PRIVATE MC_IOC_LIBRARY)

include(${CMAKE_SOURCE_DIR}/cmake/Install.cmake)
