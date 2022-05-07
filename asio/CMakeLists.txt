cmake_minimum_required(VERSION 3.10)
project(Asio VERSION 1.0.0)

add_library(Asio INTERFACE)

target_include_directories(Asio INTERFACE 
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include> 
    $<INSTALL_INTERFACE:${INCLUDE_INSTALL_DIR}>
    )

install(TARGETS Asio
   EXPORT AsioTargets
   LIBRARY DESTINATION lib  
   ARCHIVE DESTINATION lib  
   RUNTIME DESTINATION bin 
   PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_PREFIX}/include  
)

install(EXPORT AsioTargets
        FILE AsioTargets.cmake
        DESTINATION ${CMAKE_INSTALL_PREFIX}/share/cmake/
        )

include(CMakePackageConfigHelpers)

configure_package_config_file(cmake/AsioConfig.cmake.in
    ${CMAKE_INSTALL_PREFIX}/share/cmake/AsioConfig.cmake
    INSTALL_DESTINATION share/cmake/${PROJECT_NAME}
    INSTALL_PREFIX ${CMAKE_INSTALL_PREFIX}
)

write_basic_package_version_file(
        ${CMAKE_INSTALL_PREFIX}/share/cmake/AsioConfigVersion.cmake
        VERSION ${PACKAGE_VERSION}
        COMPATIBILITY AnyNewerVersion  
        )

install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/include/
    DESTINATION ${CMAKE_INSTALL_PREFIX}/include
)