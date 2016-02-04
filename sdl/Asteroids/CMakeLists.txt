cmake_minimum_required (VERSION 2.6)
project (asteroids)

set(SOURCE_FILES Asteroids.cpp
                 GameMath.cpp
                 Logger.cpp
                 ImageInfo.cpp
                 ImageCache.cpp
                 Graphics.cpp
                 Mixer.cpp
                 GameEntity.cpp
                 GraphicEntity.cpp
                 Scene.cpp
                 Command.cpp
                 TitleScene.cpp
                 ShootingScene.cpp
                 Spaceship.cpp
                 SpaceshipCommands.cpp
                 XYPair.cpp
                 TiledImage.cpp
                 Joystick.cpp
                 TextImage.cpp
                 PauseScene.cpp
                 SpaceRock.cpp
                 AnimationDriver.cpp
                 CollisionManager.cpp)

add_executable(asteroids ${SOURCE_FILES} Asteroids.cpp)

INCLUDE(FindPkgConfig)

 
PKG_SEARCH_MODULE(SDL2 REQUIRED sdl2)
PKG_SEARCH_MODULE(SDL2IMAGE REQUIRED SDL2_image>=2.0.0)
PKG_SEARCH_MODULE(SDL2TTF REQUIRED SDL2_ttf>=2.0.0)
PKG_SEARCH_MODULE(SDL2MIXER REQUIRED SDL2_mixer>=2.0.0)

INCLUDE_DIRECTORIES(${SDL2_INCLUDE_DIRS} ${SDL2IMAGE_INCLUDE_DIRS} ${SDL2TTF_INCLUDE_DIRS} ${SDL2MIXER_INCLUDE_DIRS})
TARGET_LINK_LIBRARIES(${PROJECT_NAME} ${SDL2_LIBRARIES} ${SDL2IMAGE_LIBRARIES} ${SDL2TTF_LIBRARIES} ${SDL2MIXER_LIBRARIES})


#add_library(libSDL2_ttf.so SDL2/SDL_ttf.h)
#add_library(libSDL2.so SD2/SDL.h)

IF(${CMAKE_SYSTEM_PROCESSOR} STREQUAL "armv7l")
    MESSAGE("Compiling with optimizations for my Tegra K1 Chromebook")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -mtune=cortex-a15 -mfpu=neon -funsafe-math-optimizations -mthumb -O3 -Ofast")
ELSE()
    MESSAGE("Processor=${CMAKE_SYSTEM_PROCESSOR}!")
    MESSAGE("Not optimizing for ARM processor")
ENDIF()

MESSAGE("Compiler flags=${CMAKE_CXX_FLAGS}")

