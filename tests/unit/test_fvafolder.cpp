#include <gtest/gtest.h>

#include "fvafolder.h"

// Test fixture for fvaFolder tests
class fvaFolderTests : public ::testing::Test {
protected:
    void SetUp() override {
        // Set up any necessary objects or test data
    }

    void TearDown() override {
        // Clean up any resources used by the tests
    }
};

// Test case for default constructor
TEST_F(fvaFolderTests, DefaultConstructorTest) {
    // Arrange
    fvaFolder folder;

    // Assert
    EXPECT_EQ(folder.eventId, 0);
    EXPECT_TRUE(folder.eventReasonPeopleIds.empty());
    EXPECT_TRUE(folder.tags.empty());
    EXPECT_TRUE(folder.linkedFolder.empty());
}

// Test case for setting event id
TEST_F(fvaFolderTests, SetEventIdTest) {
    // Arrange
    fvaFolder folder;

    // Act
    folder.eventId = 123;

    // Assert
    EXPECT_EQ(folder.eventId, 123);
}

// Test case for adding event reason people ids
TEST_F(fvaFolderTests, AddEventReasonPeopleIdsTest) {
    // Arrange
    fvaFolder folder;

    // Act
    folder.eventReasonPeopleIds.push_back(1);
    folder.eventReasonPeopleIds.push_back(2);
    folder.eventReasonPeopleIds.push_back(3);

    // Assert
    EXPECT_EQ(folder.eventReasonPeopleIds.size(), 3u);
    EXPECT_EQ(folder.eventReasonPeopleIds[0], 1);
    EXPECT_EQ(folder.eventReasonPeopleIds[1], 2);
    EXPECT_EQ(folder.eventReasonPeopleIds[2], 3);
}

// Test case for setting folder tags
TEST_F(fvaFolderTests, SetTagsTest) {
    // Arrange
    fvaFolder folder;

    // Act
    folder.tags = "tag1,tag2,tag3";

    // Assert
    EXPECT_EQ(folder.tags, "tag1,tag2,tag3");
}

// Test case for setting linked folder
TEST_F(fvaFolderTests, SetLinkedFolderTest) {
    // Arrange
    fvaFolder folder;

    // Act
    folder.linkedFolder = "linked_folder";

    // Assert
    EXPECT_EQ(folder.linkedFolder, "linked_folder");
}