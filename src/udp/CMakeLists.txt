file(GLOB INCLUDES ${CMAKE_CURRENT_SOURCE_DIR}/*.hpp)
file(GLOB SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)

add_library(udp STATIC ${SOURCES} ${INCLUDES} ${UIS})

target_include_directories(udp PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/..)
target_include_directories(udp PRIVATE ${Qt6Widgets_INCLUDE_DIRS})

target_link_libraries(udp PRIVATE Qt6::Core Qt6::Network)

source_group("" FILES ${SOURCES} ${INCLUDES} ${UIS})
