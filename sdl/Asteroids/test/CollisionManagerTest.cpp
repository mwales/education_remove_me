#include <gtest/gtest.h>
#include <Logger.h>
#include <random>

#include "ICollidable.h"
#include "CollisionManager.h"

class DumbTestObj : public ICollidable
{
public:
   DumbTestObj(float x, float y):
      _position{x,y}
   {
      // empty
   }

   XYPair _position;

   std::vector<CollisionRect> GetCollisionBoxes() const
   {
      CollisionRect cr = { {_position[0] - 5, _position[1] - 5},
                           {10, 10} };

      std::vector<CollisionRect> retVal;
      retVal.push_back(cr);
      return retVal;
   }

   XYPair GetPosition() const
   {
      return _position;
   }
};

class CollisionManagerTest : public testing::Test
{
public:
   CollisionManagerTest();

   ~CollisionManagerTest();

   void SetUp();

   void TearDown();

   const int COLLISION_AREA_MAX { 1000 };

   std::vector<ICollidable const *> objectAList;

   std::vector<ICollidable const *> objectBList;

   CollisionManager* mgr1;

   CollisionManager* mgr2;


};

CollisionManagerTest::CollisionManagerTest()
{
   LOG_DEBUG() << "Test Fixture Constructor Called";

   // Build up a random set of bodies and thrown them in a map to collide

}

CollisionManagerTest::~CollisionManagerTest()
{
   LOG_DEBUG() << "Test Fixture Destructor Called";
}

void CollisionManagerTest::SetUp()
{
   LOG_DEBUG() << "Test Fixture Set Up Called";
   std::default_random_engine generator;
   std::uniform_int_distribution<int> distribution(0, COLLISION_AREA_MAX -1);

   for (int i = 0; i < 1000; i++)
   {
      objectAList.push_back( new DumbTestObj(distribution(generator), distribution(generator)));
      objectBList.push_back( new DumbTestObj(distribution(generator), distribution(generator)));
   }

   // Compare 2 collision managers collision results
   mgr1 = new CollisionManager(COLLISION_AREA_MAX,
                                                COLLISION_AREA_MAX,
                                                50,
                                                objectAList,
                                                objectBList);

   mgr2 = new CollisionManager(COLLISION_AREA_MAX,
                                                COLLISION_AREA_MAX,
                                                50,
                                                objectAList,
                                                objectBList);


}

void CollisionManagerTest::TearDown()
{
   LOG_DEBUG() << "Test Fixture Tear Down Called";
}

TEST_F(CollisionManagerTest, ExponentialTest)
{
   LOG_DEBUG() << "Simple Test Body";
   ASSERT_EQ(2, 1+1);

   mgr1->ClearCollisions();
   mgr2->ClearCollisions();

   mgr1->SetCollisionManagerMode(CollisionManager::CollisionMode::EXPONENTIAL);


   mgr1->SetCollisionManagerMode(CollisionManager::CollisionMode::EXPONENTIAL);

   mgr1->CheckForCollisions();
   mgr2->CheckForCollisions();

   std::vector<Collision> mgr1Collisions = mgr1->GetCollisions();
   std::vector<Collision> mgr2Collisions = mgr2->GetCollisions();

   LOG_DEBUG() << "Number of collisions found: " << mgr1Collisions.size() << " and "
               << mgr2Collisions.size();

   for(auto curCollision : mgr1Collisions)
   {
      auto result = std::find(mgr2Collisions.begin(), mgr2Collisions.end(), curCollision);

      if (result == mgr2Collisions.end())
      {
         LOG_DEBUG() << "Details of failure";
         LOG_DEBUG() << "A: " << curCollision.first->GetPosition();
         LOG_DEBUG() << "B: " << curCollision.second->GetPosition();
      }

      ASSERT_TRUE(result != mgr2Collisions.end());
   }




}

TEST_F(CollisionManagerTest, GridTest)
{
   LOG_DEBUG() << "Simple Test Body";
   ASSERT_EQ(2, 1+1);

   mgr1->ClearCollisions();
   mgr2->ClearCollisions();

   mgr1->SetCollisionManagerMode(CollisionManager::CollisionMode::EXPONENTIAL);


   mgr2->SetCollisionManagerMode(CollisionManager::CollisionMode::GRID);

   mgr1->CheckForCollisions();
   mgr2->CheckForCollisions();

   std::vector<Collision> mgr1Collisions = mgr1->GetCollisions();
   std::vector<Collision> mgr2Collisions = mgr2->GetCollisions();

   LOG_DEBUG() << "Number of collisions found: " << mgr1Collisions.size() << " and "
               << mgr2Collisions.size();

   for(auto curCollision : mgr1Collisions)
   {
      auto result = std::find(mgr2Collisions.begin(), mgr2Collisions.end(), curCollision);

      if (result == mgr2Collisions.end())
      {
         LOG_DEBUG() << "Details of failure";
         LOG_DEBUG() << "A: " << curCollision.first->GetPosition();
         LOG_DEBUG() << "B: " << curCollision.second->GetPosition();
      }

      ASSERT_TRUE(result != mgr2Collisions.end());
   }

}

TEST_F(CollisionManagerTest, SecondSimpleTest)
{
   LOG_DEBUG() << "Second Simple Test";
   ASSERT_LT(1,2);
}

TEST(NonFixtureTestCaseColMgr, DumbTest)
{
   LOG_DEBUG() << "Dumb Test Body";
   ASSERT_GT(3,2);
}
