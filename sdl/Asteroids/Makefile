# OBJS specifies files to compile
OBJS = Asteroids.cpp GameMath.cpp Logger.cpp ImageInfo.cpp Graphics.cpp Mixer.cpp GameEntity.cpp GraphicEntity.cpp MovingEntity.cpp Scene.cpp TitleScene.cpp ShootingScene.cpp

OBJ_NAME = asteroids

CC = g++

COMPILER_FLAGS = -Wall -g

LINKER_FLAGS = -lSDL2 -lSDL2_image -lSDL2_mixer

all: $(OBJS)
	$(CC) $(OBJS) $(COMPILER_FLAGS) $(LINKER_FLAGS) -o $(OBJ_NAME)


