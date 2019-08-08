cmake_minimum_required (VERSION 2.6)
project (sg)

set(TEST_FILES scaled_graphic.cpp)

add_executable(sg ${TEST_FILES} ${EMULAT_FILES} )
INCLUDE(FindPkgConfig)

# Include SDL
find_package(sdl2 REQUIRED)
target_include_directories(sg PUBLIC ${SDL2_INCLUDE_DIRS})
target_link_libraries(sg ${SDL2_LIBRARIES})

# Modern C++ support
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g --std=c++11 -Wall")

MESSAGE("Compiler flags=${CMAKE_CXX_FLAGS}")



