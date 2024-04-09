#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "CmdLineBaseTask.h"

// Mock implementation of CmdLineBaseTask for testing purposes
class MockCmdLineBaseTask : public CmdLineBaseTask
{
public:
    MOCK_METHOD(FVA_EXIT_CODE, execute, (const CLTContext&), (override));
};

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

// Run all the tests
int main(int argc, char** argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}