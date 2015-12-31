#include <gtest/gtest.h>
#include <Logger.h>

class BuildTest : public testing::Test
{
public:
    BuildTest();

    ~BuildTest();

    void SetUp();

    void TearDown();


};

BuildTest::BuildTest()
{
    LOG_DEBUG() << "Test Fixture Constructor Called";
}

BuildTest::~BuildTest()
{
    LOG_DEBUG() << "Test Fixture Destructor Called";
}

void BuildTest::SetUp()
{
    LOG_DEBUG() << "Test Fixture Set Up Called";
}

void BuildTest::TearDown()
{
    LOG_DEBUG() << "Test Fixture Tear Down Called";
}

TEST_F(BuildTest, SimpleTest)
{
    LOG_DEBUG() << "Simple Test Body";
    ASSERT_EQ(2, 1+1);
}

TEST_F(BuildTest, SecondSimpleTest)
{
    LOG_DEBUG() << "Second Simple Test";
    ASSERT_LT(1,2);
}

TEST(NonFixtureTestCase, DumbTest)
{
    LOG_DEBUG() << "Dumb Test Body";
    ASSERT_GT(3,2);
}
