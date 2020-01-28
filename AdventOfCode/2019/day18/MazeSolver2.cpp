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

      // On the way to the key, we may pick up extra keys
      std::vector<char> keysWePickupOnWay = theMaze->getAutomaticKeys(*ktgIt);

      // Recurisvely call self  with one less keyToGet and one extra keyHave
      std::set<char> nextKeysToGet = updateKeysToGet(keysToGet, keysWePickupOnWay);
      std::vector<char> nextKeysHave = updateKeysHave(keysHave, keysWePickupOnWay);

      MAZEDEBUG << "  Keys we had before: " << theMaze->charListToString(keysHave) << std::endl;
      MAZEDEBUG << "  Keys we picked up: " << theMaze->charListToString(keysWePickupOnWay) << std::endl;
      MAZEDEBUG << "  Keys we have now: " << theMaze->charListToString(nextKeysHave) << std::endl;

      MAZEDEBUG << "  Keys we were searching for: " << theMaze->charListToString(keysToGet) << std::endl;
      MAZEDEBUG << "  Keys we are now searching for: " << theMaze->charListToString(nextKeysToGet) << std::endl;



      std::vector<MazePath> pathsAfterCurKey;
      pathsAfterCurKey = distToGetFollowingKeys(nextKeysToGet, nextKeysHave, *ktgIt);
      int curDist = theMaze->getKeyDistances(keyAtCurrentLoc)[*ktgIt];

      // Add the paths after the current key to retVal
      bool first = true;
      MazePath best;
      for(auto it = pathsAfterCurKey.begin(); it != pathsAfterCurKey.end(); it++)
      {
         MAZEDEBUG << "CurKey=" << *ktgIt << ", curDist=" << curDist << ":  " << mazePathToString(*it) << std::endl;
         it->theLength += curDist;
         //it->thePath.insert(it->thePath.begin(), nextKeysHave.begin(), nextKeysHave.end());
         it->thePath = appendKeyPath(it->thePath, nextKeysHave);
         //it ->thePath = nextKeysHave;

         MAZEDEBUG << "  to " << mazePathToString(*it) << std::endl;

         if (it->theLength == 0)
         {
            // Invalid result, ignore
            assert(false);
            continue;
         }

         if (first)
         {
            first = false;
            best = *it;
            retVal.push_back(*it);
         }
         else
         {
            if (it->theLength < best.theLength)
            {
               best = *it;
               retVal.clear();
               retVal.push_back(*it);
            }
         }

         retVal.push_back(*it);
      }

   }

   return retVal;
}

std::vector<char> MazeSolver2::appendKeyPath(std::vector<char> const & original,
                                             std::vector<char> const & append)
{
   std::vector<char> retVal;
   for(auto it = original.begin(); it != original.end(); it++)
   {
      if (isKeyInList(*it, retVal))
      {
         continue;
      }

      retVal.push_back(*it);
   }

   for(auto it = append.begin(); it != append.end(); it++)
   {
      if (isKeyInList(*it, retVal))
      {
         continue;
      }

      retVal.push_back(*it);
   }

   return retVal;
}

bool MazeSolver2::isKeyInList(char needle, std::vector<char> const & keyList)
{
   for(auto it = keyList.begin(); it != keyList.end(); it++)
   {
      if (needle == *it)
      {
         return true;
      }
   }

   return false;
}

std::set<char> MazeSolver2::updateKeysToGet(std::set<char> const & oldKeysToGet,
                                            std::vector<char> const & autoFoundKeys)
{
   std::set<char> retVal;
   for(auto it = oldKeysToGet.begin(); it != oldKeysToGet.end(); it++)
   {
      if (!isKeyInList(*it, autoFoundKeys))
      {
         // We didn't find this key, keep searching for it
         retVal.insert(*it);
      }
   }

   return retVal;
}

std::vector<char> MazeSolver2::updateKeysHave(std::vector<char> const & oldKeyList,
                                              std::vector<char> const & autoFoundKeys)
{
   std::vector<char> retVal;

   // Make sure list only has one of each key
   for(auto it = oldKeyList.begin(); it != oldKeyList.end(); it++)
   {
      if (!isKeyInList(*it, retVal))
      {
         // This is a new key that we found
         retVal.push_back(*it);
      }
   }

   for(auto it = autoFoundKeys.begin(); it != autoFoundKeys.end(); it++)
   {
      if (!isKeyInList(*it, retVal))
      {
         // This is a new key that we found
         retVal.push_back(*it);
      }
   }

   return retVal;
}
