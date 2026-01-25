#include <gtest/gtest.h>
#include "FVABaseDictionaryItem.h"

// Test fixture for fvaBaseDictionaryItem tests
class FVABaseDictionaryItemTests : public ::testing::Test
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

// Test case for default constructor
TEST_F(FVABaseDictionaryItemTests, DefaultConstructor)
{
    // Arrange
    fvaBaseDictionaryItem item;

    // Assert
    EXPECT_EQ(item.ID, 0);
    EXPECT_EQ(item.type, 0);
    EXPECT_EQ(item.name.toStdString(), "");
}

// Test case for parameterized constructor
TEST_F(FVABaseDictionaryItemTests, ParameterizedConstructor)
{
    // Arrange
    int ID = 1;
    int type = 2;
    QString name = "Test Item";
    fvaBaseDictionaryItem item(ID, type, name);

    // Assert
    EXPECT_EQ(item.ID, ID);
    EXPECT_EQ(item.type, type);
    EXPECT_EQ(item.name.toStdString(), name.toStdString());
}
