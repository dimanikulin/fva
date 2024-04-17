#include <gtest/gtest.h>
#include "../fvaFile.h"

// Test fixture for fvaFile tests
class fvaFileTests : public ::testing::Test
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
TEST_F(fvaFileTests, DefaultConstructorTest)
{
    // Arrange
    fvaFile file;

    // Assert
    EXPECT_EQ(file.eventId, FVA_UNDEFINED_ID);
    EXPECT_EQ(file.placeId, FVA_UNDEFINED_ID);
    EXPECT_EQ(file.deviceId, FVA_UNDEFINED_ID);
    EXPECT_EQ(file.scanerId, FVA_UNDEFINED_ID);
    EXPECT_TRUE(file.peopleIds.empty());
    EXPECT_TRUE(file.description.isEmpty());
    EXPECT_TRUE(file.comment.isEmpty());
    EXPECT_TRUE(file.name.isEmpty());
    EXPECT_TRUE(file.eventPeopleIds.empty());
}

// Test case for setting values
TEST_F(fvaFileTests, SetValuesTest)
{
    // Arrange
    fvaFile file;
    unsigned int eventId = 1;
    unsigned int placeId = 2;
    unsigned int deviceId = 3;
    unsigned int scanerId = 4;
    QVector<unsigned int> peopleIds = {5, 6, 7};
    QString description = "Test description";
    QString comment = "Test comment";
    QString name = "Test name";
    QVector<unsigned int> eventPeopleIds = {8, 9, 10};

    // Act
    file.eventId = eventId;
    file.placeId = placeId;
    file.deviceId = deviceId;
    file.scanerId = scanerId;
    file.peopleIds = peopleIds;
    file.description = description;
    file.comment = comment;
    file.name = name;
    file.eventPeopleIds = eventPeopleIds;

    // Assert
    EXPECT_EQ(file.eventId, eventId);
    EXPECT_EQ(file.placeId, placeId);
    EXPECT_EQ(file.deviceId, deviceId);
    EXPECT_EQ(file.scanerId, scanerId);
    EXPECT_EQ(file.peopleIds, peopleIds);
    EXPECT_EQ(file.description, description);
    EXPECT_EQ(file.comment, comment);
    EXPECT_EQ(file.name, name);
    EXPECT_EQ(file.eventPeopleIds, eventPeopleIds);
}