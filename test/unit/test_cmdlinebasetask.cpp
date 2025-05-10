#include <gtest/gtest.h>
#include "CmdLineBaseTask.h"
#include "mocks.h"

// Test fixture for CmdLineBaseTask tests
class CmdLineBaseTaskTests : public ::testing::Test
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

// Test case for execute() method
TEST_F(CmdLineBaseTaskTests, ExecuteTest)
{
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
TEST_F(CmdLineBaseTaskTests, ProcessFolderRecursivlyTest)
{
    // Arrange
    QString folder = "/path/to/folder";
    CLTContext context;
    MockCmdLineBaseTask task;

    // Act
    FVA_EXIT_CODE result = task.processFolderRecursivly(folder, context);

    // Assert
    EXPECT_EQ(result, FVA_NO_ERROR);
}


// Test case for supportReadOnly() method
TEST_F(CmdLineBaseTaskTests, SupportReadOnlyTest)
{
    // Arrange
    MockCmdLineBaseTask task;

    // Act
    bool result = task.supportReadOnly();

    // Assert
    EXPECT_FALSE(result);
}


// Test case for processFolderRecursivly when folder contains no subfolders or files
TEST(CmdLineBaseTaskTests, ProcessFolderRecursivly_NoSubfoldersOrFiles)
{
    // Arrange
    QString folder = "/path/to/folder";
    CLTContext context;
    MockCmdLineBaseTask task;

    // Act
    FVA_EXIT_CODE result = task.processFolderRecursivly(folder, context);

    // Assert
    EXPECT_EQ(result, 0);
}

// Test case for processFolderRecursivly when folder contains subfolders and files
TEST(CmdLineBaseTaskTests, ProcessFolderRecursivly_WithSubfoldersAndFiles)
{
    // Arrange
    QString folder = "/path/to/folder";
    CLTContext context;
    MockCmdLineBaseTask task;

    // Create subfolders and files
    QDir().mkpath(folder + "/subfolder1");
    QDir().mkpath(folder + "/subfolder2");
    QFile().open(folder + "/file1.txt", QIODevice::WriteOnly);
    QFile().open(folder + "/file2.txt", QIODevice::WriteOnly);

    // Act
    FVA_EXIT_CODE result = task.processFolderRecursivly(folder, context);

    // Assert
    EXPECT_EQ(result, 0);
}

// Test case for processFolderRecursivly when folder contains internal folders to skip
TEST(CmdLineBaseTaskTests, ProcessFolderRecursivly_SkipInternalFolders)
{
    // Arrange
    QString folder = "/path/to/folder";
    CLTContext context;
    MockCmdLineBaseTask task;

    // Create internal folders to skip
    QDir().mkpath(folder + "/#internal1#");
    QDir().mkpath(folder + "/#internal2#");

    // Act
    FVA_EXIT_CODE result = task.processFolderRecursivly(folder, context);

    // Assert
    EXPECT_EQ(result, 0);
}

// Test case for processFolderRecursivly when execute returns an error code
TEST(CmdLineBaseTaskTests, ProcessFolderRecursivly_ExecuteError)
{
    // Arrange
    QString folder = "/path/to/folder";
    CLTContext context;
    MockCmdLineBaseTask task;

    // Act
    FVA_EXIT_CODE result = task.processFolderRecursivly(folder, context);

    // Assert
    EXPECT_EQ(result, FVA_ERROR_INPUT_DIR_NOT_EXIST_ARG);
}