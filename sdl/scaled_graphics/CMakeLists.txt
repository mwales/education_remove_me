cmake_minimum_required (VERSION 2.6)
project (sg)


set(TEST_FILES scaled_graphic.cpp)


#PKG_SEARCH_MODULE(SDL2 REQUIRED sdl2)

# PKG_SEARCH_MODULE(SDL2NET REQUIRED SDL2_net>=2.0.0)

add_executable(sg ${TEST_FILES} ${EMULAT_FILES} )
INCLUDE(FindPkgConfig)

# Include SDL
find_package(SDL2 REQUIRED)

set(SDL2_net_LIBRARIES "-lSDL2_net")

target_compile_definitions(sg PRIVATE SDL_INCLUDED)

target_link_libraries(sg ${SDL2_LIBRARIES})

# Modern C++ support
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g --std=c++11 -Wall")

MESSAGE("Compiler flags=${CMAKE_CXX_FLAGS}")



