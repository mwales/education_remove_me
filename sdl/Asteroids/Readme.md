# Introduction #

Took a Interactive Python Programming class offered on Coursera over the summer, and the final 
project culminated into us developing a Asteroids clone called RiceRocks.  My i5 quad-core
running Chrome could play my version decently well, but have found many systems with less
specs that were playable (the runtime environment was a Javascript based python interpreter
with a graphics library called SimpleGUI).

Im now looking to reimplement everything using SDL, and hoping that the native performance
of the game will far exceed what I was seeing with browser based version.

## Credits ##

### Assets below were provided for the Interactive Python Programming Coursera course I took ###

art assets created by Kim Lathrop, may be freely re-used in non-commercial projects, please credit Kim
    
debris images - debris1_brown.png, debris2_brown.png, debris3_brown.png, debris4_brown.png
                debris1_blue.png, debris2_blue.png, debris3_blue.png, debris4_blue.png, debris_blend.png

* http://commondatastorage.googleapis.com/codeskulptor-assets/lathrop/debris2_blue.png



nebula images - nebula_brown.png, nebula_blue.png
 * http://commondatastorage.googleapis.com/codeskulptor-assets/lathrop/nebula_blue.f2014.png

splash image
* http://commondatastorage.googleapis.com/codeskulptor-assets/lathrop/splash.png

ship image
* http://commondatastorage.googleapis.com/codeskulptor-assets/lathrop/double_ship.png

missile image - shot1.png, shot2.png, shot3.png
* http://commondatastorage.googleapis.com/codeskulptor-assets/lathrop/shot2.png

asteroid images - asteroid_blue.png, asteroid_brown.png, asteroid_blend.png
* http://commondatastorage.googleapis.com/codeskulptor-assets/lathrop/asteroid_blue.png

animated explosion - explosion_orange.png, explosion_blue.png, explosion_blue2.png, explosion_alpha.png
* http://commondatastorage.googleapis.com/codeskulptor-assets/lathrop/explosion_alpha.png

sound assets purchased from sounddogs.com, please do not redistribute
* http://commondatastorage.googleapis.com/codeskulptor-assets/sounddogs/soundtrack.mp3
* http://commondatastorage.googleapis.com/codeskulptor-assets/sounddogs/missile.mp3
* http://commondatastorage.googleapis.com/codeskulptor-assets/sounddogs/thrust.mp3
* http://commondatastorage.googleapis.com/codeskulptor-assets/sounddogs/explosion.mp3

### Assets not included in the Coursera course ###

extra life sound effect
* CC 3.0 sound file by Jesus Lastra, http://opengameart.org/content/8-bit-pickup-1
* http://mwales.net/junk/SFX_Pickup_44.mp3

Mario 256 Font
* 100% FREE
* http://www.dafont.com/super-mario-256.font
* Created by fsuarez913

## Build Instructions ##

Instructions are for Ubuntu:

    sudo apt-get install build-essential git cmake
    sudo apt-get install libsdl2-dev libsdl2-mixer-dev libsdl2-ttf-dev libsdl2-image-dev

    git clone
    cd education/sdl/Asteroids
    mkdir build
    cd build
    cmake ../
    make -j8

## Documentation Generation ##

Additional documentation like class descriptions and documentation can be generated using
doxygen.

    # Packages required for documentation generation
    sudo apt-get install doxygen graphviz

    cd doc
    doxygen doxygen.config

    # Open a web browser to doc/html/index.html
    
