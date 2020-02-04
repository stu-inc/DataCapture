file(GLOB INCLUDES ${CMAKE_CURRENT_SOURCE_DIR}/*.hpp)
file(GLOB SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)

add_library(serial STATIC ${SOURCES} ${INCLUDES} ${UIS})

target_include_directories(serial PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/..)
target_include_directories(serial PRIVATE ${Qt5Widgets_INCLUDE_DIRS})

target_link_libraries(serial PRIVATE Qt5::Core Qt5::SerialPort)

source_group("" FILES ${SOURCES} ${INCLUDES} ${UIS})
