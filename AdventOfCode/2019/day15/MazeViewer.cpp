#include <iostream>
#include <vector>
#include <ncurses.h>
#include <unistd.h>

#include "MazeCommon.h"
#include "ElfComputer.h"
#include "MazeViewer.h"

const int VIEW_WIDTH = 20;
const int VIEW_HEIGHT = 20;

MazeViewer::MazeViewer():
   theDroidPosition(0,0),
   theNcursesViewOpen(true),
   theDebugEnabled(true)
{
   // nothing
   initscr();

   int row, col;
   getmaxyx(stdscr, row, col);

   if ( (col < 20) || (row < 20) )
   {
      endwin();

      std::cout << "Max rows = " << row << " and max col = " << col << std::endl;
      std::cout << "Maze Viewer requires 20x20 screen" << std::endl;
   }

   theScreenWidth = col;
   theScreenHeight = row;
   // std::cout << "Max rows = " << row << " and max col = " << col << std::endl;
   std::string debugMsg;
   debugMsg += "NCurses Screen Size = ";
   debugMsg += std::to_string(col);
   debugMsg += " x ";
   debugMsg += std::to_string(row);
   onScreenDebug(debugMsg);
}

MazeViewer::~MazeViewer()
{
   endwin();
   theNcursesViewOpen = false;

   std::cout << "Maze Viewer done" << std::endl;

   // for(auto it = theMazeData.begin(); it != theMazeData.end(); it++)
   // {
   //    std::cout << it->first.toString() << " = " << it->second << std::endl;
   // }
}



void MazeViewer::printScreen()
{
   if (!theNcursesViewOpen)
   {
      // Don't do anything here if the view already closed
      return;
   }

   int startXView = theDroidPosition.x() - VIEW_WIDTH / 2;
   int endXView = startXView + VIEW_WIDTH;
   int startYView = theDroidPosition.y() - VIEW_HEIGHT / 2;
   int endYView = startYView + VIEW_HEIGHT;

   int curRow = 0;
   int curCol = 0;

   for(int y = 0; y < VIEW_HEIGHT; y++)
   {
      for(int x = 0; x < VIEW_WIDTH; x++)
      {
         char curPoint = getMazeView(startXView + x, endYView - y);

         mvprintw(y, x, "%c", curPoint);
      }
   }

   refresh();

   usleep(10000);

}

void MazeViewer::closeViewer()
{
   theNcursesViewOpen = false;
   endwin();
}

void MazeViewer::addPoint(int x, int y, enum MazeData md)
{
   // std::cerr << "MazeViewer::addPoint(" << x << "," << y << ","
   //           << md << ")" << std::endl;

   Coord c(x, y);
   theMazeData[c] = md;
}

void MazeViewer::setDroidPosition(int x, int y)
{
   Coord c(x, y);
   theDroidPosition = c;

   if (theNcursesViewOpen)
   {
      mvprintw(VIEW_HEIGHT + 2, 0, "Droid @ %d, %d        ", x, y);
   }
}

void MazeViewer::onScreenDebug(std::string text)
{
   if (!theDebugEnabled)
   {
       return;
   }

   if (theDebugMessages.size() >= 10)
   {
      theDebugMessages.pop_back();
   }

   theDebugMessages.insert(theDebugMessages.begin(), text);

   for(int i = 0; i < theDebugMessages.size(); i++)
   {
      mvprintw(VIEW_HEIGHT + 4 + i, 0, "%s",
               theDebugMessages[i].c_str());

      // Blank out the rest of the line following the debug text
      for(int spaceNum = theDebugMessages[i].size();
          spaceNum < (theScreenWidth - 1); spaceNum++)
      {
         mvprintw(VIEW_HEIGHT + 4 + i, spaceNum, " ");
      }
   }
}

void MazeViewer::setDebugFlag(bool debugEnable)
{
   theDebugEnabled = debugEnable;
}

void MazeViewer::printWholeMap()
{
   Coord topLeft, bottomRight;
   getMapLimits(topLeft, bottomRight);

   for(int y = topLeft.y(); y >= bottomRight.y(); y--)
   {
      for(int x = topLeft.x(); x <= bottomRight.x(); x++)
      {
         char curChar = getMazeView(x,y);
         std::cout << curChar;
      }

      std::cout << std::endl;
   }
}

void MazeViewer::getMapLimits(Coord & topLeft, Coord & bottomRight)
{
   topLeft.setX(0);
   topLeft.setY(0);
   bottomRight.setX(0);
   bottomRight.setY(0);

   for(auto it = theMazeData.begin(); it != theMazeData.end(); it++)
   {
      if (it->first.x() < topLeft.x())
      {
         topLeft.setX(it->first.x());
      }

      if (it->first.x() > bottomRight.x())
      {
         bottomRight.setX(it->first.x());
      }

      if (it->first.y() > topLeft.y())
      {
         topLeft.setY(it->first.y());
      }

      if (it->first.y() < bottomRight.y())
      {
         bottomRight.setY(it->first.y());
      }
   }
}


char MazeViewer::getMazeView(int x, int y)
{
   Coord c(x,y);

   if (c == theDroidPosition)
   {
      return 'D';
   }

   auto res = theMazeData.find(c);

   if (res == theMazeData.end())
   {
      return ' ';
   }
   else
   {
      switch(theMazeData[c])
      {
      case EMPTY:
         return '.';

      case WALL:
         return '#';

      case GOAL:
         return 'X';

      case DROID:
         return 'd';

      default:
         return '?';
      }
   }

}

