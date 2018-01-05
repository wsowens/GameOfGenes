# Game of Genes
An implementation of Conway's game of life in C++ (and SDL), with some fun extra features.
## Background
This project was orignally used as an end-of-semester project for COP3503 (Programming II) at UF. The original authors were Erika Bryant, Swathi Chittlur, Katarina Jurczyk, John Kearney, Joseph Komskis, Jonathan Peritz, and me. The project used ncurses, a library for terminal-based windows, panels, etc. However, this project was difficult to install on Windows environments. Moreover, the project was limited by its reliance on text and a limited number of colors. Thus, after learning some SDL2, I decided to remake this project with SDL2.

## Installation
### Pre-made Installation
There are premade executables (with the necessary Dynamically Linked Libraries) available in the "Release" section of this repository.
The following flavors are available for each version:
* Windows 32-bit [RELEASE NAME]/win32
* Windows 64-bit [RELEASE NAME]/win64
* Linux 64-bit   [RELEASE NAME]/linux

As of writing, the Linux version is not thoroughly tested. For anyone having difficulties, compiling on Linux is relatively straightforward (see section below).
If for some reason you are having difficulties with the SDL runtime, you can re-download the .DLLs here: [https://www.libsdl.org/download-2.0.php]

### Compiling from Source
All the executables above were compiled on Linux. The Linux executable was compiled with g++, and the Windows executables were cross-compiled with [mingw-w64](http://mingw-w64.org/doku.php/start).

There are many tutorials available online, but if you have any questions, hit me up @xeranimus.
Here's a checklist of what you need to do:
* Make sure you have the development kits for SDL2, SDL2_image, and SDL2_ttf installed
* If you are using the makefile below, make that you link to the correct libraries.
* Make sure you can start the SDL2 runtime properly (that may mean putting the proper .DLLs in the folder on Windows, or making sure the package installed correctly on Linux).

A Makefile is provided with the following options:
* make WIN64
* make WIN32
* make LINUX

Finally, a few tips for anyone compiling from source:
* Users on Windows may have to change #include <SDL2/SDL.h> and other similar calls (e.g. <SDL2/SDL_img.h>) to SDL header files to #include <SDL.h>
* You may need to change the compiler in the Makefile.
* I statically linked the C++ and C standard library to avoid the need for those DLLs.
* There are a number of tutorials available online. I suggest looking at [Lazy Foo' Productions](http://lazyfoo.net/tutorials/SDL/06_extension_libraries_and_loading_other_image_formats/index.php), they do a better job explaining than I could.
* Contact me @xeranimus if you have any issues. 

## Documentation / Usage
Upon start up, the a main menu is displayed. The following options are available:
* Create New Board:			Create an empty board with X height and Y width.  All cells will be dead.
* Load Saved Board:			Load a saved board from [Game Of Genes]/saved
* Create Random Board:		Create a randomized board (i.e. Each cell will be randomly turned on or off according to a proportion provided by the user.
* Enter Pattern Editor:		Enters the pattern editor. The pattern editor is essentially the same as a board editor, except the user cannot run any iterations. It is used exclusively for making patterns to load into other boards.
* Exit						Closes the program.

Note: that sometimes the user may be prompted asking if they "want to enable wraparound". Wraparound means that cells at the edges of the board touch one another.

All boards and patterns are saved under [Game of Genes Directory]/saved/
Whenever you attempt to load a pattern or board, GameOfGenes looks under this directory.
To begin with, there are several hundred .rle files from the [Life Wiki](http://www.conwaylife.com/wiki/Main_Page).

At any point, press "H" with a board/pattern open to a help menu with the controls.

### "Classic Mode"
"Classic mode" works on the standard rules of the original game of life by John Conway. (Cells with 3 neighbors are born, cells with exactly 2 or 3 neighbors survive.)

### "Differentiated Mode" (Coming Soon)
There are a number of different cells, each with the same properties.

### "Genetic Mode" (Coming Soon)
Each cell has a set of genes. Any cell born as a neighbor of the of a given cell may inherit some of that cell's genes. 

## Controls
### GENERAL
* +, =				Zoom in
* —					Zoom out
* Arrow Keys			Move Cursor / Pan Camera
* Scrollwheel 		Zoom in / Out
* Right-Click			Pan Camera
* R					Reset Zoom
* H					Show Help Menu
 
### PAUSED MODE
* Spacebar			Toggle Cell
* Left-Click			Toggle Cell
* Enter/Return		Perform One Iteration
* ] (Right Bracket)	Increase Speed
* [ (Left Bracket)	Decrease Speed
* Left-Click			Toggle Cell
* P					Play (enter running mode)
* A					Place pattern
* ESC					Main Menu

### RUNNING MODE
* ] (Right Bracket)	Increase Speed
* [ (Left Bracket)	Decrease Speed
* P					Pause (enter paused mode)
* ESC					Exit to Menu

### PLACE MODE
* Spacebar			Place Pattern
* Left-Click			Place Pattern
* Enter/Return		Place Pattern
* ] (Right Bracket)	Rotate Right
* [ (Left Bracket)	Rotate Left
* A, ESC				Exit Place Mode

### EDIT MODE
* Spacebar			Toggle Cell
* Left-Click			Toggle Cell
* A					Place pattern
* ESC					Main Menu

## Credits
Thank you to John Conway, whose work spans many areas beyond the game that bears his name.

I have to thank the original authors of the semester project that this was forked from:
Erika Bryant, Swathi Chittlur, Katarina Jurczyk, John Kearney, Joseph Komskis, and Jonathan Peritz

The developers of SDL2 and zlib, the libraries that this project uses. Both these libraries are distributed under the zlib license.

Ubuntu Mono is distributed under the Ubuntu Font License.