#include <iostream>
#include <string>
#include <sstream>
#include <map>
#include <utility>
#include <vector>
#include <set>
#include <random>
#include <algorithm>
#include <iterator>

// 2017 Google Code Jam Round 1A - Alphabet Cake
// I was really worried about a simple algorithm for this challenge getting stuck and not being able
// to get a solution.  So I implemented this really weird random algorithm where it just tries over
// over to get a solution.  It worked on the small data set, but got hung up and couldn't solve the
// large data set that I received during Code Jam.

// Problem
// 
// You are catering a party for some children, and you are serving them a cake in the shape of a grid with R rows and C columns. Your assistant has started to decorate the cake by writing every child's initial in icing on exactly one cell of the cake. Each cell contains at most one initial, and since no two children share the same initial, no initial appears more than once on the cake.
// 
// Each child wants a single rectangular (grid-aligned) piece of cake that has their initial and no other child's initial(s). Can you find a way to assign every blank cell of the cake to one child, such that this goal is accomplished? It is guaranteed that this is always possible. There is no need to split the cake evenly among the children, and one or more of them may even get a 1-by-1 piece; this will be a valuable life lesson about unfairness.
// 
// Input
// 
// The first line of the input gives the number of test cases, T. T test cases follow. Each begins with one line with two integers R and C. Then, there are R more lines of C characters each, representing the cake. Each character is either an uppercase English letter (which means that your assistant has already added that letter to that cell) or ? (which means that that cell is blank).
// 
// Output
// 
// For each test case, output one line containing Case #x: and nothing else. Then output R more lines of C characters each. Your output grid must be identical to the input grid, but with every ? replaced with an uppercase English letter, representing that that cell appears in the slice for the child who has that initial. You may not add letters that did not originally appear in the input. In your grid, for each letter, the region formed by all the cells containing that letter must be a single grid-aligned rectangle.
// 
// If there are multiple possible answers, you may output any of them.
// 

struct cakeCoordinate
{
   int x;
   int y;
};
 
bool expand(char cake[25][25], int rows, int cols, char child, int directionX, int directionY)
{
   std::vector<struct cakeCoordinate> expPts;

   for(int x = 0; x < cols; x++)
   {
      for(int y = 0; y < rows; y++)
      {
         if (cake[x][y] == child)
         {
            int expandX = x + directionX;
            int expandY = y + directionY;

            struct cakeCoordinate expansionPoint;
            expansionPoint.x = expandX;
            expansionPoint.y = expandY;

            expPts.push_back(expansionPoint);
         }
      }
   }

   for(auto singlePoint = expPts.begin(); singlePoint != expPts.end(); singlePoint++)
   {
      cake[singlePoint->x][singlePoint->y] = child;
   }
}

std::string cakeToString(char cake[25][25], int rows, int cols)
{
   std::string retVal;
   for(int y = 0; y < rows; y++)
   {
      for(int x = 0; x < cols; x++)
      {

         retVal += cake[x][y];
      }

      retVal += '\n';
   }

   return retVal;
}


bool canChildExpand(char cake[25][25], int rows, int cols, char child, int directionX, int directionY)
{
   //std::cerr << "canChildExpand(" << child << ", x=" << directionX << ", y=" << directionY << " ... ";

   for(int x = 0; x < cols; x++)
   {
      for(int y = 0; y < rows; y++)
      {
         if (cake[x][y] == child)
         {
            int expandX = x + directionX;
            int expandY = y + directionY;

            // Can this child be expanded ?
            if ( (expandX < 0) || (expandX >= cols))
            {
               // Expand out of x range
               //std::cerr << "Nope" << std::endl;
               return false;
            }

            if ( (expandY < 0) || (expandY >= rows))
            {
               // Expand out of y range
               //std::cerr << "Nope" << std::endl;
               return false;
            }

            if ( (cake[expandX][expandY] != '?') && (cake[expandX][expandY] != child) )
            {
               // Space already taken by some other dumb kids initials
               //std::cerr << "Nope" << std::endl;
               return false;
            }
         }
      }
   }

   //std::cerr << "YES" << std::endl;
   return true;
}

bool isDecorated(char cake[25][25], int rows, int cols)
{
   for(int x = 0; x < cols; x++)
   {
      for(int y = 0; y < rows; y++)
      {
         if (cake[x][y] == '?')
            return false;
      }
   }

   return true;
}

void rotateDirections(std::vector<struct cakeCoordinate>& points)
{
   if (points.size() <= 1)
      return;

   struct cakeCoordinate firstCoord = points.front();

   points.erase(points.begin());
   points.push_back(firstCoord);
}

void rotateChildren(std::vector<char>& children)
{
   if (children.size() <= 1)
      return;

   char firstChild = children.front();

   children.erase(children.begin());
   children.push_back(firstChild);
}

std::string childrenToString(std::vector<char> children)
{
   std::string retVal;
   for(auto childIter = children.begin(); childIter != children.end(); childIter++)
   {
      retVal += *childIter;
   }

   return retVal;
}

void copyCake(char dest[25][25], char src[25][25], int rows, int cols)
{
   for(int x = 0; x < cols; x++)
   {
      for(int y = 0; y < rows; y++)
      {
         dest[x][y] = src[x][y];
      }
   }
}

std::string solveCase()
{
   int rows, cols;
   std::cin >> rows;
   std::cin >> cols;

   char cake[25][25];
   char cakeCopy[25][25];

   std::vector<char> children;

   for(int curRow = 0; curRow < rows; curRow++)
   {
      for(int curCol = 0; curCol < cols; curCol++)
      {
         char nextInitial;
         std::cin >> nextInitial;
         cake[curCol][curRow] = nextInitial;

         if (nextInitial != '?')
         {
            children.push_back(nextInitial);
         }
      }
   }

   std::cerr << "Current Cake:" << std::endl << cakeToString(cake, rows, cols) << std::endl;



   std::cerr << "Children: " << childrenToString(children) << std::endl;

   // Going to shuffle the order of the children over and over until I get a working solution
   std::random_device rd;
   std::mt19937 g(rd());

   std::vector<struct cakeCoordinate> expandDirs;
   expandDirs.push_back({1,0});
   expandDirs.push_back({0,1});
   expandDirs.push_back({-1,0});
   expandDirs.push_back({0,-1});


   bool solved = false;
   while(!solved)
   {
      std::shuffle(children.begin(), children.end(), g);

      //std::cerr << "Children: " << childrenToString(children) << std::endl;

      bool didAnyExpand = true;
      copyCake(cakeCopy, cake, rows, cols);
      while(didAnyExpand)
      {
         didAnyExpand = false;
         int numChildren = children.size();
         for(int expandAttempts = 0; expandAttempts < numChildren * expandDirs.size(); expandAttempts++)
         {

            if (isDecorated(cakeCopy, rows, cols))
            {
               solved = true;
               break;
            }

            rotateChildren(children);

            if ( (expandAttempts % numChildren) == 0)
            {
               rotateDirections(expandDirs);
            }

            if (canChildExpand(cakeCopy, rows, cols, children.front(), expandDirs.front().x, expandDirs.front().y))
            {
               expand(cakeCopy, rows, cols, children.front(), expandDirs.front().x, expandDirs.front().y);
               //std::cerr << "Cake after expand!" << std::endl << cakeToString(cakeCopy, rows, cols) << std::endl;
               didAnyExpand = true;
            }
         }
      }

      if (!solved && !didAnyExpand)
      {
         std::cerr << ".";
         std::shuffle(expandDirs.begin(), expandDirs.end(), g);
         //return "FAILED";
      }



   }

   return cakeToString(cakeCopy , rows, cols);

}

int main(int, char**)
{
   int numCases = 0;
   std::cin >> numCases;

   for(int i = 0; i < numCases; i++)
   {
      std::cout << "Case #" << i+1 << ": " << std::endl << solveCase();
   }

   return 0;
}
