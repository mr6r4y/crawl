cmake_minimum_required(VERSION 3.15)
project(crawl C)

find_package(CURL REQUIRED)
find_package(tidy-html5 REQUIRED)

add_executable(${PROJECT_NAME} src/main.c)
target_link_libraries(${PROJECT_NAME} CURL::libcurl)
target_link_libraries(${PROJECT_NAME} tidy-html5::tidy-html5)
