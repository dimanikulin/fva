#include <gtest/gtest.h>
#include "fvaperson.h"

// Test fixture for fvaPerson tests
class FVAPersonTests : public ::testing::Test
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


// Test case for fvaPerson class
TEST_F(FVAPersonTests, Constructor)
{
    // Act
    fvaPerson person;

    // Arrange
    person.Id = 1;
    person.name = "John";
    person.fullName = "John Doe";
    person.type = 2;
    person.institution = 3;
    
    // Assert
    EXPECT_EQ(person.Id, 1);
    EXPECT_EQ(person.name, "John");
    EXPECT_EQ(person.fullName, "John Doe");
    EXPECT_EQ(person.type, 2);
    EXPECT_EQ(person.institution, 3);
}