#include <iostream>
#include <string>
#include <sstream>
#include <map>
#include <utility>
#include <vector>

// Google Code Jam 2017 Qualification Round Problem - Bathroom Stals

// Problem
// 
// A certain bathroom has N + 2 stalls in a single row; the stalls on the left and right ends are permanently occupied by the bathroom guards. The other N stalls are for users.
// 
// Whenever someone enters the bathroom, they try to choose a stall that is as far from other people as possible. To avoid confusion, they follow deterministic rules: For each empty stall S, they compute two values LS and RS, each of which is the number of empty stalls between S and the closest occupied stall to the left or right, respectively. Then they consider the set of stalls with the farthest closest neighbor, that is, those S for which min(LS, RS) is maximal. If there is only one such stall, they choose it; otherwise, they choose the one among those where max(LS, RS) is maximal. If there are still multiple tied stalls, they choose the leftmost stall among those.
// 
// K people are about to enter the bathroom; each one will choose their stall before the next arrives. Nobody will ever leave.
// 
// When the last person chooses their stall S, what will the values of max(LS, RS) and min(LS, RS) be?
// 
// Solving this problem
// 
// This problem has 2 Small datasets and 1 Large dataset. You must solve the first Small dataset before you can attempt the second Small dataset. You will be able to retry either of the Small datasets (with a time penalty). You will be able to make a single attempt at the Large, as usual, only after solving both Small datasets.
// 
// Input
// 
// The first line of the input gives the number of test cases, T. T lines follow. Each line describes a test case with two integers N and K, as described above.
// 
// Output
// 
// For each test case, output one line containing Case #x: y z, where x is the test case number (starting from 1), y is max(LS, RS), and z is min(LS, RS) as calculated by the last person to enter the bathroom for their chosen stall S.

// One of the more memorable and fun problems I've encountered in my Code Jam experience.  I didn't
// have the time that I would require for solving the large data set for this problem, so I just
// resigned that I would only try to get the small and medium data set solutions, which would be 
// sufficient for normal sized integers (as I write this though, I just realized that I think the 
// large data set was bound at 10^18, which would be OK for a uint64_t.

// I also realized that the proper solution for this would probably require someone figure out the
// actual pattern of how bathroom stalls are filled, and solve it that way.  But the much more fun
// solution would be to actually simulate how a bathroom gets filled in given just the rules
// stated.

// I didn't realize this though, this simulation would not be sufficent to solve even the medium
// size data set.

// I apologize in advance for all the "crappy" variable names
struct toiletStall
{
   bool occupied;
   int position;
   int emptyShittersLeft;
   int emptyShittersRight;
   int closestNeighborDist;
   int farthestNeighborDist;
};

typedef struct toiletStall Toilet;

Toilet* findEmptyShitter(std::vector<Toilet*>* br)
{
   std::vector<Toilet*> prospects;

   for(auto pottyIter = br->begin(); pottyIter != br->end(); pottyIter++)
   {
      if ((*pottyIter)->occupied)
         continue;

      //std::cerr << "Toilet #" << (*pottyIter)->position << " is unoccupied" << std::endl;
      if (prospects.empty())
      {
         // Well, this is better than using the sink right?
         prospects.push_back(*pottyIter);
         //std::cerr << "Well, " << (*pottyIter)->position << " is better than the sink! " << (*pottyIter)->closestNeighborDist << std::endl;
      }
      else
      {
         // Is this better than the other toilets?
         if (prospects.front()->closestNeighborDist > (*pottyIter)->closestNeighborDist)
         {
            //std::cerr << "Closest person to stall #" <<  (*pottyIter)->position << " is to close! " << (*pottyIter)->closestNeighborDist << std::endl;
         }
         else if (prospects.front()->closestNeighborDist == (*pottyIter)->closestNeighborDist)
         {
            //std::cerr << "Another good prospect at #" << (*pottyIter)->position << std::endl;
            prospects.push_back(*pottyIter);
         }
         else
         {
            // new best toilet
            //std::cerr << "Toilet " << (*pottyIter)->position << " is the new best! " << (*pottyIter)->closestNeighborDist << std::endl;
            prospects.clear();

            prospects.push_back(*pottyIter);
         }
      }
   }

   if (prospects.size() == 1)
   {
      //std::cerr << "One candidate out shits the rest: " << prospects.front()->position << std::endl;
      return prospects.front();
   }

   Toilet* retVal = prospects.front();
   for(int i = 1; i < prospects.size(); i++)
   {
      if (retVal->farthestNeighborDist < prospects[i]->farthestNeighborDist)
      {
         retVal = prospects[i];
      }
   }

   //std::cerr << "It was close, but we are choosing toilet " << retVal->position << std::endl;
   return retVal;
}

void updateMinAndMax(Toilet* t)
{
   if (t->emptyShittersLeft > t->emptyShittersRight)
   {
      t->closestNeighborDist = t->emptyShittersRight;
      t->farthestNeighborDist = t->emptyShittersLeft;
   }
   else
   {
      t->closestNeighborDist = t->emptyShittersLeft;
      t->farthestNeighborDist = t->emptyShittersRight;
   }
}

void insertNewShitter(std::vector<Toilet*>* br, int position)
{
   Toilet* winner = (*br)[position];
   winner->occupied = true;
   winner->emptyShittersLeft = -1;
   winner->emptyShittersRight = -1;
   winner->farthestNeighborDist = -1;
   winner->closestNeighborDist = -1;

   for(int i = position - 1; i >= 0; i--)
   {
      // Updating toilets to the left of our stall
      Toilet* ts = (*br)[i];

      if (!ts->occupied)
      {
         ts->emptyShittersRight = position - i - 1;
         updateMinAndMax(ts);
      }
      else
      {
         // Find one occupied, we are done
         break;
      }
   }

   for(int i = position + 1; i < br->size(); i++)
   {
      // Updating toilets to the right of our stall
      Toilet* ts = (*br)[i];

      if (!ts->occupied)
      {
         ts->emptyShittersLeft = i - position - 1;
         updateMinAndMax(ts);
      }
      else
      {
         // Find one occupied, we are done
         break;
      }
   }
}

void initializeToilets(std::vector<Toilet*>* br, int numToilets)
{
   for(int i = 0; i < numToilets; i++)
   {
      Toilet* ts = new Toilet();
      ts->occupied = false;
      ts->emptyShittersLeft = i;
      ts->emptyShittersRight = numToilets - i - 1;
      ts->position = i;

      updateMinAndMax(ts);

      br->push_back(ts);
   }
}

void cleanBathroom(std::vector<Toilet*>* br)
{
   while(!br->empty())
   {
      Toilet* t = br->back();
      br->pop_back();
      delete t;
   }
}

void printBathroomDetails(std::vector<Toilet*>* br)
{
   for(auto stall = br->begin(); stall != br->end(); stall++)
   {
      std::cerr << "Toilet #" << (*stall)->position << ", occupied=" << ((*stall)->occupied ? "Yes" : "No") << ", EmptyLeft="
                << (*stall)->emptyShittersLeft << ", EmptyRight=" << (*stall)->emptyShittersRight << ", Min="
                << (*stall)->closestNeighborDist << ", Max=" << (*stall)->farthestNeighborDist << std::endl;
   }
}


std::string solveCase()
{
   int numTurdReceivers = 0;
   std::cin >> numTurdReceivers;

   std::vector<Toilet*> bathroom;
   initializeToilets(&bathroom, numTurdReceivers);

   //printBathroomDetails(&bathroom);

   int numPeopleDeucing = 0;
   std::cin >> numPeopleDeucing;

   int retMin;
   int retMax;
   for(int i = 0; i < numPeopleDeucing; i++)
   {
      Toilet* anEmpty = findEmptyShitter(&bathroom);
      retMin = anEmpty->closestNeighborDist;
      retMax = anEmpty->farthestNeighborDist;

      insertNewShitter(&bathroom, anEmpty->position);

      //printBathroomDetails(&bathroom);
   }

   std::ostringstream oss;
   oss << retMax << " " << retMin;

   cleanBathroom(&bathroom);

   return oss.str();

}

int main(int, char**)
{
   int numCases = 0;
   std::cin >> numCases;

   for(int i = 0; i < numCases; i++)
   {
      std::cout << "Case #" << i+1 << ": " << solveCase() << std::endl;
   }

   return 0;
}
