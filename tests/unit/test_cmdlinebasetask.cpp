#include <gtest/gtest.h>

#include <filesystem>
#include <fstream>

#include "CmdLineBaseTask.h"
#include "mocks.h"

// Test fixture for CmdLineBaseTask tests
class CmdLineBaseTaskTests : public ::testing::Test {
protected:
    void SetUp() override {
        // Set up any necessary objects or test data
    }

    void TearDown() override {
        // Clean up any resources used by the tests
    }
};

// Test case for execute() method
TEST_F(CmdLineBaseTaskTests, ExecuteTest) {
    // Arrange
    CLTContext context;
    MockCmdLineBaseTask task;

    // Set up expectations on the task
    EXPECT_CALL(task, execute(context)).Times(1);

    // Act
    FVA_EXIT_CODE result = task.execute(context);

    // Assert
    EXPECT_EQ(result, FVA_NO_ERROR);
}

// Test case for processFolderRecursivly() method
TEST_F(CmdLineBaseTaskTests, ProcessFolderRecursivlyTest) {
    // Arrange
    std::string folder = TEST_DIR;
    CLTContext context;
    MockCmdLineBaseTask task;

    // Act
    FVA_EXIT_CODE result = task.processFolderRecursivly(folder, context);

    // Assert
    EXPECT_EQ(result, FVA_NO_ERROR);
}

// Test case for supportReadOnly() method
TEST_F(CmdLineBaseTaskTests, SupportReadOnlyTest) {
    // Arrange
    MockCmdLineBaseTask task;

    // Act
    bool result = task.supportReadOnly();

    // Assert
    EXPECT_FALSE(result);
}

// Test case for processFolderRecursivly when folder contains no subfolders or files
TEST_F(CmdLineBaseTaskTests, ProcessFolderRecursivly_NoSubfoldersOrFiles) {
    // Arrange
    std::string folder = TEST_DIR;
    CLTContext context;
    MockCmdLineBaseTask task;

    // Act
    FVA_EXIT_CODE result = task.processFolderRecursivly(folder, context);

    // Assert
    EXPECT_EQ(result, 0);
}

// Test case for processFolderRecursivly when folder contains subfolders and files
TEST_F(CmdLineBaseTaskTests, ProcessFolderRecursivly_WithSubfoldersAndFiles) {
    CLTContext context;
    MockCmdLineBaseTask task;

    // Create subfolders and files
    std::filesystem::create_directories(std::string(TEST_DIR) + "/subfolder1");
    std::filesystem::create_directories(std::string(TEST_DIR) + "/subfolder2");
    std::ofstream(std::string(TEST_DIR) + "/file1.txt");
    std::ofstream(std::string(TEST_DIR) + "/subfolder1/file2.txt");

    // Act
    FVA_EXIT_CODE result = task.processFolderRecursivly(TEST_DIR, context);

    // Assert
    EXPECT_EQ(result, 0);
}

// Test case for processFolderRecursivly when folder contains internal folders to skip
TEST_F(CmdLineBaseTaskTests, ProcessFolderRecursivly_SkipInternalFolders) {
    CLTContext context;
    MockCmdLineBaseTask task;

    // Create internal folders to skip
    std::filesystem::create_directories(std::string(TEST_DIR) + "/#internal1#");
    std::filesystem::create_directories(std::string(TEST_DIR) + "/#internal2#");

    // Act
    FVA_EXIT_CODE result = task.processFolderRecursivly(TEST_DIR, context);

    // Assert
    EXPECT_EQ(result, 0);
}
