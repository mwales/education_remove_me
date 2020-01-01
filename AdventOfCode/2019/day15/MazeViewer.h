#ifndef MAZE_VIEWER_H
#define MAZE_VIEWER_H

#include <iostream>
#include <vector>
#include <map>

#include "ElfComputer.h"

// View is going to be 20x20 box
// Debug text and position underneath the screen

class Coord : public std::pair<int, int>
{
public:

   Coord(int x, int y);

   std::string toString() const;

   int x() const { return first; }
   int y() const { return second; }

};

class MazeViewer
{

public:

   enum MazeData
   {
      EMPTY,
      WALL,
      GOAL,
      DROID
   };

   MazeViewer();
   ~MazeViewer();


   void printScreen();

   void addPoint(int x, int y, enum MazeData);

   void setDroidPosition(int x, int y);

   void onScreenDebug(std::string text);

protected:

   char getMazeView(int x, int y);

   std::map<Coord, enum MazeData> theMazeData;

   Coord theDroidPosition;

   int theScreenWidth;

   int theScreenHeight;

   std::vector<std::string> theDebugMessages;
};

#endif
