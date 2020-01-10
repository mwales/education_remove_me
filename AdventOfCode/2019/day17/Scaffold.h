#ifndef SCAFFOLD_H
#define SCAFFOLD_H

#include <vector>
#include <stdint.h>

class Scaffold
{
public:

   Scaffold(std::vector<int64_t> const & data);

   bool isPointIntersection(int x, int y);

   int getSumAlignmentParameters();

   int getData(int x, int y);

   int getWidth();

   int getHeight();

private:

   std::vector<int64_t> theData;

   int theWidth;

   int theHeight;
};

#endif
