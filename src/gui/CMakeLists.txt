file(GLOB INCLUDES ${CMAKE_CURRENT_SOURCE_DIR}/*.hpp)
file(GLOB SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)
file(GLOB UIS ${CMAKE_CURRENT_SOURCE_DIR}/*.ui)

add_library(gui STATIC ${SOURCES} ${INCLUDES} ${UIS})

target_include_directories(gui PRIVATE ${PROJECT_BINARY_DIR})
target_include_directories(gui PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/..)
target_include_directories(gui PRIVATE ${Qt5Widgets_INCLUDE_DIRS})

target_link_libraries(gui PRIVATE serial udp)
target_link_libraries(gui PRIVATE Qt5::Widgets Qt5::Network Qt5::SerialPort)

source_group("" FILES ${SOURCES} ${INCLUDES} ${UIS})
