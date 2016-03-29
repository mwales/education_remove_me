#include <gtest/gtest.h>

#include "Logger.h"
#include "GameMath.h"

class MathTest : public testing::Test
{
public:
    MathTest();

    ~MathTest();

    void SetUp();

    void TearDown();


};

MathTest::MathTest()
{
    LOG_DEBUG() << "Test Fixture Constructor Called";
}

MathTest::~MathTest()
{
    LOG_DEBUG() << "Test Fixture Destructor Called";
}

void MathTest::SetUp()
{
    LOG_DEBUG() << "Test Fixture Set Up Called";
}

void MathTest::TearDown()
{
    LOG_DEBUG() << "Test Fixture Tear Down Called";
}

TEST_F(MathTest, AnglesTest)
{
    LOG_DEBUG() << "Test converting vectors to angles";
    ASSERT_EQ(45.0, GameMath::VectorAngleDeg(XYPair(5,5)));
    ASSERT_EQ(135.0, GameMath::VectorAngleDeg(XYPair(-5,5)));
    ASSERT_EQ(225.0, GameMath::VectorAngleDeg(XYPair(-5,-5)));
    ASSERT_EQ(315.0, GameMath::VectorAngleDeg(XYPair(5,-5)));
}

TEST_F(MathTest, SecondSimpleTest)
{
    LOG_DEBUG() << "Second Simple Test";
    ASSERT_LT(1,2);
}

TEST(NonFixtureTestCase, DumbTestMath)
{
    LOG_DEBUG() << "Dumb Test Body";
    ASSERT_GT(3,2);
}
