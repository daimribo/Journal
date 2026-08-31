cmake_minimum_required(VERSION 3.28)
project(journal)

set(CMAKE_C_STANDARD 23)
set(CMAKE_C_STANDARD_REQUIRED ON)

find_package(PkgConfig REQUIRED)
pkg_check_modules(GTK3 REQUIRED gtk+-3.0)

add_executable(journal)

target_sources(journal PRIVATE main.c)
target_include_directories(journal PRIVATE ${GTK3_INCLUDE_DIRS})
target_link_directories(journal PRIVATE ${GTK3_LIBRARY_DIRS})
target_link_libraries(journal PRIVATE ${GTK3_LIBRARIES})

install(TARGETS journal DESTINATION bin)
