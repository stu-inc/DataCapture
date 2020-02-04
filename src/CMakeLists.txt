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

add_executable(SerialCapture ${APP_BUNDLE} ${SOURCES} ${INCLUDES})

set_target_properties(SerialCapture PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR})

if(APPLE)
    set_target_properties(SerialCapture PROPERTIES MACOSX_BUNDLE_INFO_PLIST ${CMAKE_CURRENT_SOURCE_DIR}/../res/mac/AppleInfo.plist)
    set(APPLE_GUI_BUNDLE_NAME SerialCapture)
    set(APPLE_GUI_EXECUTABLE SerialCapture)
    set(APPLE_GUI_BUNDLE_VERSION ${PROJECT_VERSION})
endif()

target_include_directories(SerialCapture PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(SerialCapture PRIVATE serial udp tcp gui task)
target_link_libraries(SerialCapture PRIVATE Qt5::Widgets Qt5::Network Qt5::SerialPort)

if(WIN32)
    add_custom_command(
        TARGET SerialCapture
        POST_BUILD
        COMMAND ${Qt5_DIR}/../../../bin/windeployqt --no-translations --no-system-d3d-compiler --no-compiler-runtime --no-angle $<TARGET_FILE:SerialCapture>
        WORKING_DIRECTORY ${PROJECT_BINARY_DIR}
    )
elseif(APPLE)
    add_custom_command(
        TARGET SerialCapture
        POST_BUILD
        COMMAND ${Qt5_DIR}/../../../bin/macdeployqt SerialCapture.app
        WORKING_DIRECTORY ${PROJECT_BINARY_DIR}
    )
endif()

source_group("" FILES ${SOURCES} ${INCLUDES})
