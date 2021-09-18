file(GLOB INCLUDES ${CMAKE_CURRENT_SOURCE_DIR}/*.hpp)
file(GLOB SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)

add_library(task STATIC ${SOURCES} ${INCLUDES} ${UIS})

target_include_directories(task PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/..)
target_include_directories(task PRIVATE ${Qt6Widgets_INCLUDE_DIRS})

target_link_libraries(task PRIVATE gui)
target_link_libraries(task PRIVATE Qt6::Core)

source_group("" FILES ${SOURCES} ${INCLUDES} ${UIS})
