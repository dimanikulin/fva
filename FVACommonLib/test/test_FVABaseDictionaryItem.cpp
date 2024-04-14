#include <gtest/gtest.h>
#include "../FVABaseDictionaryItem.h"

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

// Test case for setting and getting ID
TEST_F(FVABaseDictionaryItemTests, SetAndGetID)
{
    // Arrange
    fvaBaseDictionaryItem item;
    int ID = 1;

    // Act
    item.setID(ID);
    int retrievedID = item.getID();

    // Assert
    EXPECT_EQ(retrievedID, ID);
}

// Test case for setting and getting type
TEST_F(FVABaseDictionaryItemTests, SetAndGetType)
{
    // Arrange
    fvaBaseDictionaryItem item;
    int type = 2;

    // Act
    item.setType(type);
    int retrievedType = item.getType();

    // Assert
    EXPECT_EQ(retrievedType, type);
}

// Test case for setting and getting name
TEST_F(FVABaseDictionaryItemTests, SetAndGetName)
{
    // Arrange
    fvaBaseDictionaryItem item;
    QString name = "Test Item";

    // Act
    item.setName(name);
    QString retrievedName = item.getName();

    // Assert
    EXPECT_EQ(retrievedName.toStdString(), name.toStdString());
}

// Run all the tests
int main(int argc, char** argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}