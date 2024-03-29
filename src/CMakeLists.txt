add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/task)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/udp)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/tcp)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/serial)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/gui)

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/CMakeAuto.txt ${PROJECT_BINARY_DIR}/autodef.hpp)

file(GLOB INCLUDES ${CMAKE_CURRENT_SOURCE_DIR}/*.hpp)
file(GLOB SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)

if(WIN32)
    set(APP_BUNDLE WIN32)
elseif(APPLE)
    set(APP_BUNDLE MACOSX_BUNDLE)
endif()

add_executable(DataCapture ${APP_BUNDLE} ${SOURCES} ${INCLUDES})

set_target_properties(DataCapture PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR})

if(APPLE)
    set_target_properties(DataCapture PROPERTIES MACOSX_BUNDLE_INFO_PLIST ${CMAKE_CURRENT_SOURCE_DIR}/../res/mac/AppleInfo.plist)
    set(APPLE_GUI_BUNDLE_NAME DataCapture)
    set(APPLE_GUI_EXECUTABLE DataCapture)
    set(APPLE_GUI_BUNDLE_VERSION ${PROJECT_VERSION})
endif()

target_include_directories(DataCapture PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(DataCapture PRIVATE serial udp tcp gui task)
target_link_libraries(DataCapture PRIVATE Qt6::Widgets Qt6::Network Qt6::SerialPort)

if(WIN32)
    add_custom_command(
        TARGET DataCapture
        POST_BUILD
        COMMAND ${Qt6_DIR}/../../../bin/windeployqt --no-translations --no-system-d3d-compiler --no-compiler-runtime --no-opengl-sw $<TARGET_FILE:DataCapture>
        WORKING_DIRECTORY ${PROJECT_BINARY_DIR}
    )
elseif(APPLE)
    add_custom_command(
        TARGET DataCapture
        POST_BUILD
        COMMAND ${Qt6_DIR}/../../../bin/macdeployqt DataCapture.app
        WORKING_DIRECTORY ${PROJECT_BINARY_DIR}
    )
endif()

source_group("" FILES ${SOURCES} ${INCLUDES})
