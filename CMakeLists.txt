cmake_minimum_required(VERSION 3.15)
project(crawl C)

set(CMAKE_C_STANDARD 90)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(CURL REQUIRED)
find_package(tidy-html5 REQUIRED)

include_directories(BEFORE src/ )
include_directories(BEFORE deps/ )

# Strip Release executable
add_link_options($<$<CONFIG:RELEASE>:-s>)

add_executable(${PROJECT_NAME} src/main.c)
target_link_libraries(${PROJECT_NAME} CURL::libcurl)
target_link_libraries(${PROJECT_NAME} tidy-html5::tidy-html5)

add_executable(test-slice test/test-slice.c)
add_executable(test-vec test/test-vec.c)
add_executable(test-veclist test/test-veclist.c)
add_executable(test-path_join test/test-path_join.c)
