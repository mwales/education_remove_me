#include "Scaffold.h"
#include <iostream>



Scaffold::Scaffold(std::vector<int64_t> const & data):
   theData(data)
{
   int w = 0;
   int h = 0;
   bool widthSet = false;

   for(auto it = theData.begin(); it != theData.end(); it++)
   {
      // Is this int a line ending?
      if (*it == 0x0a)
      {
         if (!widthSet)
         {
            theWidth = w;
            widthSet = true;
         }

         h++;

      }

      w++;
   }

   // There is a double new-line at the end of the data
   theHeight = h - 1;

   std::cout << "Determined the scaffold to be " << theWidth << " x "
             << theHeight << std::endl;
}

bool Scaffold::isPointIntersection(int x, int y)
{
   // Is the point at the left or the right edge?
   if ( (x <= 0) || (x >= theWidth) )
   {
      return false;
   }

   // Is the point at the top or bottom?
   if ( (y <= 0) || (y >= theHeight) )
   {
      return false;
   }

   // The points of interest must all have scaffold or the droid
   // Or more easily, not just be empty!
   std::vector<std::pair<int, int> > poi;
   poi.push_back(std::make_pair(x, y));
   poi.push_back(std::make_pair(x+1, y));
   poi.push_back(std::make_pair(x-1, y));
   poi.push_back(std::make_pair(x, y-1));
   poi.push_back(std::make_pair(x, y+1));

   // Verify none of those points are '.'
   for(auto it = poi.begin(); it != poi.end(); it++)
   {
      if (getData(it->first, it->second) == 46)
      {
         return false;
      }
   }

   // If we got this far the point is either scaffold or droid
   return true;

}

int Scaffold::getSumAlignmentParameters()
{
   int curVal = 0;
   int sum = 0;
   for(int y = 0; y < theHeight; y++)
   {
      for(int x = 0; x < theWidth; x++)
      {
         if (isPointIntersection(x,y))
         {
            curVal = x * y;
            sum += curVal;
         }
      }
   }

   return sum;
}

int Scaffold::getData(int x, int y)
{
   // each row has all the cells + newline
   int index = y * (theWidth + 1) + x;

   if (index > theData.size())
   {
      std::cerr << "Invalid data asked for (" << x << "," << y << ")" << std::endl;
      return -1;
   }

   return theData.at(index);
}

int Scaffold::getWidth()
{
   return theWidth;
}

int Scaffold::getHeight()
{
   return theHeight;
}
