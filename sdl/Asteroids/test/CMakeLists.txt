cmake_minimum_required (VERSION 2.6)
project (astest)

# Used to get detailed compiler output
set(CMAKE_VERBOSE_MAKEFILE ON)

set(SOURCE_FILES astest.cpp
                 BuildTest.cpp
                 ../Logger.cpp
                 ../XYPair.cpp) 

add_executable(astest ${SOURCE_FILES} ${TEST_FILES})

INCLUDE(FindPkgConfig)

# Test project needs google test
INCLUDE(FindGTest)
IF(GTEST_FOUND)
    MESSAGE("GTest Found at ${GTEST_INCLUDE_DIRS}")
ELSE()
    MESSAGE("GTest not found")
ENDIF()

PKG_SEARCH_MODULE(SDL2 REQUIRED sdl2)
PKG_SEARCH_MODULE(SDL2IMAGE REQUIRED SDL2_image>=2.0.0)
PKG_SEARCH_MODULE(SDL2TTF REQUIRED SDL2_ttf>=2.0.0)
PKG_SEARCH_MODULE(SDL2MIXER REQUIRED SDL2_mixer>=2.0.0)

INCLUDE_DIRECTORIES(${SDL2_INCLUDE_DIRS} ${SDL2IMAGE_INCLUDE_DIRS} ${SDL2TTF_INCLUDE_DIRS} ${SDL2MIXER_INCLUDE_DIRS} ${GTEST_INCLUDE_DIRS})

# Include the main game files
INCLUDE_DIRECTORIES(../)

TARGET_LINK_LIBRARIES(${PROJECT_NAME} ${SDL2_LIBRARIES} ${SDL2IMAGE_LIBRARIES} ${SDL2TTF_LIBRARIES} ${SDL2MIXER_LIBRARIES} ${GTEST_BOTH_LIBRARIES} -lpthread)


IF(${CMAKE_SYSTEM_PROCESSOR} STREQUAL "armv7l")
    MESSAGE("Compiling with optimizations for my Tegra K1 Chromebook")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -mtune=cortex-a15 -mfpu=neon -funsafe-math-optimizations -mthumb -O3 -Ofast")
ELSE()
    MESSAGE("Processor=${CMAKE_SYSTEM_PROCESSOR}!")
    MESSAGE("Not optimizing for ARM processor")
ENDIF()

MESSAGE("Compiler flags=${CMAKE_CXX_FLAGS}")

