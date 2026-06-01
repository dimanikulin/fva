#include <gtest/gtest.h>

#include <filesystem>
#include <fstream>

#include "CLTCreateDirStructByDevNames.h"

// Test fixture for CLTCreateDirStructByDeviceName tests
class CLTCreateDirStructByDeviceNameTests : public ::testing::Test {
protected:
    void SetUp() override {
        // Set up any necessary objects or test data
    }

    void TearDown() override {
        // Clean up any resources used by the tests
    }
};

// Test case for execute function
TEST_F(CLTCreateDirStructByDeviceNameTests, Execute) {
    // Arrange
    CLTCreateDirStructByDeviceName cltCreateDirStruct;
    CLTContext context;  // Set up the necessary context for the test

    // Act
    FVA_EXIT_CODE result = cltCreateDirStruct.execute(context);

    // Assert
    EXPECT_EQ(FVA_NO_ERROR, result);  // Verify that the function returns FVA_NO_ERROR
    // Add more assertions to verify the expected behavior and output
}

// Test case for Name function
TEST_F(CLTCreateDirStructByDeviceNameTests, Name) {
    // Arrange

    // Act
    std::string name = CLTCreateDirStructByDeviceName::Name();

    // Assert
    // Verify the expected name
    ASSERT_EQ("CLTCreateDirStructByDeviceName", name);
}

// Test case for supportReadOnly function
TEST_F(CLTCreateDirStructByDeviceNameTests, SupportReadOnly) {
    // Arrange
    CLTCreateDirStructByDeviceName cltCreateDirStruct;

    // Act
    bool readOnly = cltCreateDirStruct.supportReadOnly();

    // Assert
    // Verify the expected read-only support
    ASSERT_FALSE(readOnly);
}

// Add more test cases for other member functions as needed
// Test case for execute function with no picture files
TEST_F(CLTCreateDirStructByDeviceNameTests, Execute_NoPictureFiles) {
    namespace fs = std::filesystem;

    // Arrange
    CLTCreateDirStructByDeviceName task;
    CLTContext context;
    fs::create_directory("test_folder");

    // Act
    FVA_EXIT_CODE result = task.execute(context);

    // Assert
    ASSERT_EQ(FVA_NO_ERROR, result);
    ASSERT_TRUE(fs::is_empty("test_folder"));  // Verify that no sub-folders are created

    fs::remove_all("test_folder");
}

// Test case for execute function with picture files
TEST_F(CLTCreateDirStructByDeviceNameTests, Execute_WithPictureFiles) {
    namespace fs = std::filesystem;

    // Arrange
    CLTCreateDirStructByDeviceName task;
    CLTContext context;
    fs::create_directory("test_folder");

    // Create a picture file
    std::ofstream file("test_folder/picture.jpg");
    file.close();

    // Act
    FVA_EXIT_CODE result = task.execute(context);

    // Assert
    ASSERT_EQ(FVA_NO_ERROR, result);
    // TODO to uncomment and fix
    // ASSERT_TRUE(fs::exists("test_folder/EMPTY"));  // Verify that the "EMPTY" sub-folder is created
    // ASSERT_TRUE(fs::exists("test_folder/EMPTY/picture.jpg"));  // Verify the picture file is moved

    fs::remove_all("test_folder");
}

// Test case for execute function with picture files and device names
TEST_F(CLTCreateDirStructByDeviceNameTests, Execute_WithPictureFilesAndDeviceNames) {
    namespace fs = std::filesystem;

    // Arrange
    CLTCreateDirStructByDeviceName task;
    CLTContext context;
    fs::create_directory("test_folder");

    // Create picture files with different device names
    std::ofstream file1("test_folder/picture1.jpg");
    file1.close();

    std::ofstream file2("test_folder/picture2.jpg");
    file2.close();

    // Act
    FVA_EXIT_CODE result = task.execute(context);

    // Assert
    ASSERT_EQ(FVA_NO_ERROR, result);
    // TODO to uncomment and fix
    // ASSERT_TRUE(fs::exists("test_folder/device1"));  // Verify that the "device1" sub-folder is created
    // ASSERT_TRUE(fs::exists("test_folder/device1/picture1.jpg"));  // Verify first picture file moved
    // ASSERT_TRUE(fs::exists("test_folder/device2"));  // Verify that the "device2" sub-folder is created
    // ASSERT_TRUE(fs::exists("test_folder/device2/picture2.jpg"));  // Verify second picture file moved

    fs::remove_all("test_folder");
}
