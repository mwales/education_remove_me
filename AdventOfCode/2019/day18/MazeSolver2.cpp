#include "MazeSolver2.h"
#include <assert.h>

//#define MAZE_SOLVER_DEBUG
#ifdef MAZE_SOLVER_DEBUG
#define MAZEDEBUG std::cout
#else
#define MAZEDEBUG if(0) std::cout
#endif

std::string mazePathToString(MazePath const & mp)
{
   std::string retVal;

   retVal = "[Len=";
   retVal += std::to_string(mp.theLength);
   retVal += ", Path=";

   bool first = true;
   for(auto it = mp.thePath.begin(); it != mp.thePath.end(); it++)
   {
      if (first)
      {
         first = false;
      }
      else
      {
         retVal += ",";
      }

      retVal += *it;
   }

   retVal += "]";

   return retVal;
}

MazeSolver2::MazeSolver2(Maze* m)
{
   theMaze = m;
}

std::vector<MazePath> MazeSolver2::distToGetFollowingKeys(std::set<char> keysToGet,
                                           std::vector<char> keysHave,
                                           char keyAtCurrentLoc)
{
   std::vector<MazePath> retVal;

   MAZEDEBUG << "distToGetFollowingKeys(get=" << theMaze->charListToString(keysToGet)
             << ", have=" << theMaze->charListToString(keysHave) << ", curKey="
             << keyAtCurrentLoc << ")" << std::endl;

   // check for recursive end case
   if (keysToGet.empty())
   {
      MAZEDEBUG << "Valid path = " << theMaze->charListToString(keysHave) << std::endl;

      // Return a list of 1 item, 0 length, no nodes
      MazePath mp;
      mp.theLength = 0;
      retVal.push_back(mp);
      return retVal;
   }

   int curMinDist;
   bool curMinDistSet = false;

   for(auto ktgIt = keysToGet.begin(); ktgIt != keysToGet.end(); ktgIt++)
   {
      MAZEDEBUG << "MS2 looping on " << *ktgIt << std::endl;

      // Can we get this key with the keys that we have currently?
      if (!theMaze->canIGetKey(*ktgIt, keysHave))
      {
         // We can't reach this key yet
         continue;
      }

      // Recurisvely call self  with one less keyToGet and one extra keyHave
      std::set<char> nextKeysToGet = keysToGet;
      std::vector<char> nextKeysHave = keysHave;

      nextKeysToGet.erase(nextKeysToGet.find(*ktgIt));
      nextKeysHave.push_back(*ktgIt);

      std::vector<MazePath> pathsAfterCurKey;
      pathsAfterCurKey = distToGetFollowingKeys(nextKeysToGet, nextKeysHave, *ktgIt);
      int curDist = theMaze->getKeyDistances(keyAtCurrentLoc)[*ktgIt];

      // Add the paths after the current key to retVal
      for(auto it = pathsAfterCurKey.begin(); it != pathsAfterCurKey.end(); it++)
      {
         MAZEDEBUG << "CurKey=" << *ktgIt << ", curDist=" << curDist << ":  " << mazePathToString(*it) << std::endl;
         it->theLength += curDist;
         it->thePath.insert(it->thePath.begin(), *ktgIt);

         MAZEDEBUG << "  to " << mazePathToString(*it) << std::endl;;

         retVal.push_back(*it);
      }

   }

   return retVal;
}
