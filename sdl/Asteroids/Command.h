#pragma once

#include <stdlib.h> // size_t
#include <vector>

/**
 * The Command pattern (see Game Design Patterns)
 *
 * All commands have an execute() method, and all commands have a data bundle
 * that can be optionally used
 */
class Command
{
public:
   Command();

   virtual ~Command();

   /// Executes the command, returns true if the command will create a state/scene change
   virtual bool Execute() = 0;

   size_t GetBundleSize() { return theBundleData.size(); }

   unsigned int GetBundleParameter(int bundleIndex) { return theBundleData[bundleIndex]; }

   void PushInBundle(unsigned int data) {theBundleData.push_back(data); }

   void ClearBundle() { theBundleData.clear(); }

protected:

   std::vector<unsigned int> theBundleData;
};


