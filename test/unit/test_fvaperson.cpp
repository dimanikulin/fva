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
    // Arrange
    int id = 1;
    QString name = "John";
    QString fullName = "John Doe";
    int type = 2;
    int institution = 3;

    // Act
    fvaPerson person(id, name, fullName, type, institution);

    // Assert
    EXPECT_EQ(person.Id, id);
    EXPECT_EQ(person.name, name);
    EXPECT_EQ(person.fullName, fullName);
    EXPECT_EQ(person.type, type);
    EXPECT_EQ(person.institution, institution);
}