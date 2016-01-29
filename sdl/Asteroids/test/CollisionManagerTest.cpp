#include <gtest/gtest.h>
#include <Logger.h>

class CollisionManagerTest : public testing::Test
{
public:
    CollisionManagerTest();

    ~CollisionManagerTest();

    void SetUp();

    void TearDown();


};

CollisionManagerTest::CollisionManagerTest()
{
    LOG_DEBUG() << "Test Fixture Constructor Called";
}

CollisionManagerTest::~CollisionManagerTest()
{
    LOG_DEBUG() << "Test Fixture Destructor Called";
}

void CollisionManagerTest::SetUp()
{
    LOG_DEBUG() << "Test Fixture Set Up Called";
}

void CollisionManagerTest::TearDown()
{
    LOG_DEBUG() << "Test Fixture Tear Down Called";
}

TEST_F(CollisionManagerTest, SimpleTest)
{
    LOG_DEBUG() << "Simple Test Body";
    ASSERT_EQ(2, 1+1);
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
