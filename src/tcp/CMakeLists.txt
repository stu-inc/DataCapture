file(GLOB INCLUDES ${CMAKE_CURRENT_SOURCE_DIR}/*.hpp)
file(GLOB SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)

add_library(tcp STATIC ${SOURCES} ${INCLUDES} ${UIS})

target_include_directories(tcp PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/..)
target_include_directories(tcp PRIVATE ${Qt5Widgets_INCLUDE_DIRS})

target_link_libraries(tcp PRIVATE Qt5::Core Qt5::Network)

source_group("" FILES ${SOURCES} ${INCLUDES} ${UIS})
