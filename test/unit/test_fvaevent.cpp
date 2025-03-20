#include <gtest/gtest.h>
#include "FVAEvent.h"

// Test fixture for fvaEvent tests
class FVAEventTests : public ::testing::Test
{
protected:
    void SetUp() override
    {
        // Set up any necessary objects or test data
    }

    void TearDown() override
    {
        // Clean up any resources used by the tests
    }
};

/*
// Test case for default constructor
TEST_F(FVAEventTests, DefaultConstructorTest)
{
    // Arrange

    // Act
    fvaEvent event;

    // Assert
    EXPECT_EQ(event.ID, 0);
    EXPECT_EQ(event.name, "");
    EXPECT_EQ(event.type, 0);
    EXPECT_EQ(event.institution, 0);
}

// Test case for parameterized constructor
TEST_F(FVAEventTests, ParameterizedConstructorTest)
{
    // Arrange
    int ID = 1;
    QString name = "Test Event";
    unsigned int type = 2;
    unsigned int institution = 3;

    // Act
    fvaEvent event(ID, name, type, institution);

    // Assert
    EXPECT_EQ(event.ID, ID);
    EXPECT_EQ(event.name, name);
    EXPECT_EQ(event.type, type);
    EXPECT_EQ(event.institution, institution);
}

// Test case for getters and setters
TEST_F(FVAEventTests, GettersAndSettersTest)
{
    // Arrange
    fvaEvent event;
    int ID = 1;
    QString name = "Test Event";
    unsigned int type = 2;
    unsigned int institution = 3;

    // Act
    event.setID(ID);
    event.setName(name);
    event.setType(type);
    event.setInstitution(institution);

    // Assert
    EXPECT_EQ(event.getID(), ID);
    EXPECT_EQ(event.getName(), name);
    EXPECT_EQ(event.getType(), type);
    EXPECT_EQ(event.getInstitution(), institution);
}
*/