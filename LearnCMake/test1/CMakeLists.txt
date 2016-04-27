cmake_minimum_required(VERSION 2.8)
project("Test1")

add_executable(demo a.cpp
                    b.cpp)

add_subdirectory(mod_c)
target_link_libraries(demo carrier destroyer)

add_subdirectory(mod_d)

