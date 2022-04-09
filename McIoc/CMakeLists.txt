project(McIoc)

include_directories(${CMAKE_SOURCE_DIR})
include_directories(${CMAKE_BINARY_DIR}/McCore)

set(PROJECT_HEADERS
    McIocMacroGlobal.h
    McIocConstantGlobal.h
    McIocGlobal.h
    McGlobal.h
    Utils/IMcBeanBuildable.h
    BeanFactory/IMcBeanFactory.h
    BeanFactory/IMcBeanBuilderRegistry.h
    BeanFactory/IMcBeanReferenceResolver.h
    BeanFactory/IMcConfigurableBeanFactory.h
    BeanFactory/Impl/McAbstractBeanFactory.h
    BeanFactory/Impl/McDefaultBeanFactory.h
    BeanBuilder/IMcBeanBuilder.h
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
    BeanBuilderReader/IMcBeanBuilderReader.h
    BeanBuilderReader/Impl/McAbstractBeanBuilderReader.h
    BeanBuilderReader/Impl/McXmlBeanBuilderReader.h
)

set(PROJECT_SOURCES
    McIocGlobal.cpp
    BeanFactory/Impl/McAbstractBeanFactory.cpp
    BeanFactory/Impl/McDefaultBeanFactory.cpp
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
    BeanBuilderReader/Impl/McAbstractBeanBuilderReader.cpp
    BeanBuilderReader/Impl/McXmlBeanBuilderReader.cpp
)

add_library(${PROJECT_NAME} ${MC_BUILD_TYPE}
    ${PROJECT_HEADERS}
    ${PROJECT_SOURCES}
)

target_link_libraries(${PROJECT_NAME} PRIVATE Qt6::Core
    McCore)

target_compile_definitions(${PROJECT_NAME} PRIVATE MC_IOC_LIBRARY)

include(${CMAKE_SOURCE_DIR}/cmake/Install.cmake)
