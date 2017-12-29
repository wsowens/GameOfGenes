#Copyright Notice:
#The files within this zip file are copyrighted by Lazy Foo' Productions (2004-2014)
#and may not be redistributed without written permission.

#OBJS specifies which files to compile as part of the project
OBJS = GameOfLife.cpp Controller.cpp Board.cpp Util.cpp Formats.cpp Pattern.cpp ButtonBox.cpp Button.cpp

#CC specifies which compiler we're using
CC = x86_64-w64-mingw32-g++

#INCLUDE_PATHS specifies the additional include paths we'll need
INCLUDE_PATHS = -IC:/usr/x86_64-w64-mingw32/include/SDL2

#LIBRARY_PATHS specifies the additional library paths we'll need
LIBRARY_PATHS = -LC:/usr/x86_64-w64-mingw32/lib

#COMPILER_FLAGS specifies the additional compilation options we're using
# -w suppresses all warnings
# -Wl,-subsystem,windows gets rid of the console window
COMPILER_FLAGS = -w -static-libgcc -static-libstdc++ -std=c++11

#LINKER_FLAGS specifies the libraries we're linking against
LINKER_FLAGS = -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -lSDL2_ttf

#OBJ_NAME specifies the name of our exectuable
OBJ_NAME = ./64bit/main.exe

#This is the target that compiles our executable
all : $(OBJS)
	$(CC) -o $(OBJ_NAME) $(OBJS) $(INCLUDE_PATHS) $(LIBRARY_PATHS) $(COMPILER_FLAGS) $(LINKER_FLAGS)

board:
	$(CC) -o ./64bit/board.exe Board.cpp Formats.cpp Util.cpp $(INCLUDE_PATHS) $(LIBRARY_PATHS) $(COMPILER_FLAGS) $(LINKER_FLAGS)