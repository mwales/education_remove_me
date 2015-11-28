#pragma once

#include <stdlib.h> // size_t
#include <vector>

class Command
{
public:
   Command();

   virtual ~Command();

   virtual void Execute() = 0;

   size_t GetBundleSize() { return _bundleData.size(); }

   unsigned int GetBundleParameter(int bundleIndex) { return _bundleData[bundleIndex]; }

   void PushInBundle(unsigned int data) {_bundleData.push_back(data); }

   void ClearBundle() { _bundleData.clear(); }

protected:

   std::vector<unsigned int> _bundleData;
};


