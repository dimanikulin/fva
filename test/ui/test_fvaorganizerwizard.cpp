#include <gtest/gtest.h>
// #include "../fvaorganizerwizard.h"

// Test fixture for FVAOrganizerWizard tests
class FVAOrganizerWizardTests : public ::testing::Test
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
// Test case for FVAOrganizerWizard construction
TEST_F(FVAOrganizerWizardTests, Construction)
{
    // Arrange
    QWidget parent;

    // Act
    FVAOrganizerWizard wizard(&parent);

    // Assert
    // Verify the expected behavior after constructing the wizard
    // Add assertions to check if the wizard is properly initialized
}


// Test case for FVAOrganizerWizard pages
TEST_F(FVAOrganizerWizardTests, Pages)
{
    // Arrange
    QWidget parent;
    FVAOrganizerWizard wizard(&parent);

    // Act
    int pageCount = wizard.pageCount();

    // Assert
    // Verify the expected behavior of the page count
    // Add assertions to check if the correct number of pages are created
    ASSERT_EQ(pageCount, 8);
}

// Test case for inputFolder function
TEST_F(FVAOrganizerWizardTests, InputFolder)
{
    // Arrange
    FVAOrganizerWizard wizard;

    // Act
    QString inputFolder = "test_folder";
    wizard.inputFolder(inputFolder);

    // Assert
    EXPECT_EQ(wizard.inputFolder(), inputFolder);
}

// Test case for matchedDeviceName function
TEST_F(FVAOrganizerWizardTests, MatchedDeviceName)
{
    // Arrange
    FVAOrganizerWizard wizard;

    // Act
    QString deviceName = "test_device";
    wizard.matchedDeviceName(deviceName);

    // Assert
    EXPECT_EQ(wizard.matchedDeviceName(), deviceName);
}

// Test case for fullDeviceMap function
TEST_F(FVAOrganizerWizardTests, FullDeviceMap)
{
    // Arrange
    FVAOrganizerWizard wizard;
    DEVICE_MAP deviceMap;

    // Act
    // Populate deviceMap with test data
    wizard.fullDeviceMap(deviceMap);

    // Assert
    EXPECT_EQ(wizard.fullDeviceMap(), deviceMap);
}

// Test case for matchedDeviceMap function
TEST_F(FVAOrganizerWizardTests, MatchedDeviceMap)
{
    // Arrange
    FVAOrganizerWizard wizard;
    DEVICE_MAP deviceMap;

    // Act
    // Populate deviceMap with test data
    wizard.matchedDeviceMap(deviceMap);

    // Assert
    EXPECT_EQ(wizard.matchedDeviceMap(), deviceMap);
}

// Test case for matchedDeviceID function
TEST_F(FVAOrganizerWizardTests, MatchedDeviceID)
{
    // Arrange
    FVAOrganizerWizard wizard;

    // Act
    int deviceID = 123;
    wizard.matchedDeviceID(deviceID);

    // Assert
    EXPECT_EQ(wizard.matchedDeviceID(), deviceID);
}

// Add more test cases for other member functions as needed
*/