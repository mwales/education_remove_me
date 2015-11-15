# OBJS specifies files to compile
OBJS = Asteroids.cpp GameMath.cpp Logger.cpp 


OBJ_NAME = asteroids

CC = g++

COMPILER_FLAGS = -Wall -g

LINKER_FLAGS = -lSDL2 -lSDL2_image

all: $(OBJS)
	$(CC) $(OBJS) $(COMPILER_FLAGS) $(LINKER_FLAGS) -o $(OBJ_NAME)


