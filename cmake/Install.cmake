export(TARGETS ${PROJECT_NAME} FILE ${CMAKE_BINARY_DIR}/${PROJECT_NAME}Targets.cmake)

include(CMakePackageConfigHelpers)

write_basic_package_version_file(
    ${CMAKE_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake
    VERSION ${MC_QUICKBOOT_VERSION}
    COMPATIBILITY SameMajorVersion)

foreach(hds lbs bns IN ZIP_LISTS MC_QUICKBOOT_INSTALL_HEADERS MC_QUICKBOOT_INSTALL_LIBS MC_QUICKBOOT_INSTALL_BINS)
    configure_package_config_file(${CMAKE_SOURCE_DIR}/cmake/ConfigTemplate.cmake.in
        ${CMAKE_BINARY_DIR}/${PROJECT_NAME}Config.cmake
        INSTALL_DESTINATION ${lbs}/cmake/${PROJECT_NAME})

    install(DIRECTORY
        ./
        DESTINATION ${hds}/${PROJECT_NAME}
        FILES_MATCHING
        PATTERN "*.h"
    )

    install(TARGETS ${PROJECT_NAME}
        EXPORT ${PROJECT_NAME}Targets
        RUNTIME DESTINATION ${bns}
        LIBRARY DESTINATION ${lbs}
        ARCHIVE DESTINATION ${lbs}
        FRAMEWORK DESTINATION ${lbs}
    )

    install(FILES $<TARGET_PDB_FILE:${PROJECT_NAME}> DESTINATION ${bns} OPTIONAL)

    install(EXPORT ${PROJECT_NAME}Targets
        DESTINATION ${lbs}/cmake/${PROJECT_NAME})

    install(FILES
        ${CMAKE_BINARY_DIR}/${PROJECT_NAME}Config.cmake
        ${CMAKE_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake
        DESTINATION ${lbs}/cmake/${PROJECT_NAME})
endforeach()
