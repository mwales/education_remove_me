#ifndef MAZE_VIEWER_H
#define MAZE_VIEWER_H

#include <iostream>
#include <vector>
#include <map>

#include "ElfComputer.h"

// View is going to be 20x20 box
// Debug text and position underneath the screen

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

   void closeViewer();

   void addPoint(int x, int y, enum MazeData);

   void setDroidPosition(int x, int y);

   void onScreenDebug(std::string text);

   void setDebugFlag(bool);

   void printWholeMap();

   void getMapLimits(Coord & topLeft, Coord & bottomRight);

protected:

   char getMazeView(int x, int y);

   std::map<Coord, enum MazeData> theMazeData;

   Coord theDroidPosition;

   int theScreenWidth;

   int theScreenHeight;

   std::vector<std::string> theDebugMessages;

   bool theNcursesViewOpen;

   bool theDebugEnabled;
};

#endif
