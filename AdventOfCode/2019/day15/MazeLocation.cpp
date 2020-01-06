#include "MazeLocation.h"

MazeLocation::MazeLocation(Coord const & loc):
   theLoc(loc),
   theContent(-1)
{
   // Default constructor
}

void MazeLocation::setContent(int mazeContent)
{
   theContent = mazeContent;
}

MazeLocation MazeLocation::createNeighbor(int direction)
{
   Coord nc(0,0);
   switch(direction)
   {
   case MAZE_NORTH:
      nc = Coord(theLoc.x(), theLoc.y() + 1);
      break;
   case MAZE_EAST:
      nc = Coord(theLoc.x() + 1, theLoc.y());
      break;
   case MAZE_SOUTH:
      nc = Coord(theLoc.x(), theLoc.y() - 1);
      break;
   case MAZE_WEST:
      nc = Coord(theLoc.x() - 1, theLoc.y());
      break;
   }

   MazeLocation retVal(nc);

   retVal.theShortestPath = theShortestPath;
   retVal.theShortestPath.push_back(direction);

   return retVal;
}

Coord MazeLocation::getLocation() const
{
   return theLoc;
}

int MazeLocation::getNumSteps() const
{
   return theShortestPath.size();
}

std::vector<int> MazeLocation::getPath() const
{
   return theShortestPath;
}

int MazeLocation::getContent() const
{
   return theContent;
}

std::string MazeLocation::toString()
{
   std::string retVal;
   retVal = "[";
   retVal += theLoc.toString();
   retVal += ",";

   switch(theContent)
   {
   case MAZE_EMPTY:
      retVal += "Empty";
      break;
   case MAZE_WALL:
      retVal += "Wall";
      break;
   case MAZE_GOAL:
      retVal += "Goal";
      break;
   default:
      retVal += "Invalid";
   }

   retVal += ",<";
   bool first = true;
   for(auto it = theShortestPath.begin(); it != theShortestPath.end(); it++)
   {
      if (first)
      {
         first = false;
      }
      else {
         retVal += ",";
      }

      switch(*it)
      {
      case MAZE_NORTH:
         retVal += "N";
         break;
      case MAZE_EAST:
         retVal += "E";
         break;
      case MAZE_SOUTH:
         retVal += "S";
         break;
      case MAZE_WEST:
         retVal += "W";
         break;
      }
   }

   retVal += ">]";
   return retVal;

}

bool operator==(MazeLocation const & lhs, MazeLocation const & rhs)
{
   return (lhs.getLocation() == rhs.getLocation());
}

bool operator!=(MazeLocation const & lhs, MazeLocation const & rhs)
{
   return !(lhs == rhs);
}
