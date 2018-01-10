#OBJS specifies which files to compile as part of the project
OBJS = GameOfGenes.cpp Controller.cpp Board.cpp Util.cpp Formats.cpp Pattern.cpp ButtonBox.cpp Button.cpp TextBox.cpp GridBox.cpp

#The different compilers used. Change these to the appropriate paths as you need. 
Win64_Compiler = x86_64-w64-mingw32-g++
Win32_Compiler = i686-w64-mingw32-g++
Linux_Compiler = g++

#the following paths were used on my system, you may need to change this for your purposes
INCLUDE_W64 = -IC:/usr/x86_64-w64-mingw32/include/SDL2
INCLUDE_W32 = -IC:/usr/i686-w64-mingw32/include/SDL2
INCLUDE_LINUX =

#LIBRARY_PATHS specifies the additional library paths we'll need
LIBRARY_W64 = -LC:/usr/x86_64-w64-mingw32/lib
LIBRARY_W32 = -LC:/usr/i686-w64-mingw32/include/SDL2 
LIBRARY_LINUX =

#COMPILER FLAGS 
# -Wl,-subsystem,windows removes console window 
# -static-libgcc and -static-libstdc++ statically link the standard c and C++ libraries on windows
# -source uses many C++11 features, thus -std=c++11
WIN_CF = -Wl,-subsystem,windows -static-libgcc -static-libstdc++ -std=c++11
LINUX_CF = -std=c++11

#LINKER_FLAGS specifies the libraries we're linking against
LINKER_FLAGS = -lSDL2main -lSDL2 -lSDL2_image -lSDL2_ttf

#OUTPUT:: make whatever you want
OUTPUT = ./GameOfGenes.exe

#For users compiling on Windows without mingw32, remove the -lmingw32 flag
WIN64 : $(OBJS)
	$(Win64_Compiler) -o $(OUTPUT) $(OBJS) $(INCLUDE_W64) $(LIBRARY_W64) $(WIN_CF) -lmingw32 $(LINKER_FLAGS)

WIN32 : $(OBJS)
	$(Win32_Compiler) -o $(OUTPUT) $(OBJS) $(INCLUDE_W32) $(LIBRARY_W32) $(WIN_CF) -lmingw32 $(LINKER_FLAGS)

LINUX : $(OBJS)
	$(Linux_Compiler) -o $(OUTPUT) $(OBJS) $(LINUX_CF) $(LINKER_FLAGS)

