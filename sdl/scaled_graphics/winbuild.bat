REM The default path for MinGW is c:\MinGW
set PATH=%PATH%;c:\MinGW\bin

REM Change this to wherever the SDL2 development libraries are installed
set SDL_PATH=c:\checkouts\sdl2\i686-w64-mingw32\

REM Order of the libraries at the end are very important
g++ scaled_graphic.cpp -I%SDL_PATH%\include\sdl2 -L%SDL_PATH%\lib -lmingw32 -lSDL2main -lSDL2

REM Keep sdl2.dll in the same directory as your executable to run
copy %SDL_PATH%\bin\sdl2.dll sdl2.dll




