#include <gtest/gtest.h>
#include "CmdLineBaseExecutor.h"
#include "FVADataProcessor.h"
#include "mocks.h"

// Test fixture for CmdLineBaseExecutor tests
class CmdLineBaseExecutorTests : public ::testing::Test
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

// Test case for the run() method
TEST_F(CmdLineBaseExecutorTests, RunTest)
{
    // Arrange
    CLTContext context;
    context.dir = "D:/a/fva/fva/build"; // Set the directory to a test value, somewhere on your system
    FvaConfiguration cfg;
    MockCmdLineBaseExecutor executor;
    MockCmdLineBaseTask mockTask;

    // Act
    FVA_EXIT_CODE result = executor.run(context, cfg);

    // Assert
    EXPECT_EQ(result, FVA_NO_ERROR);
}

// Test case for run() when input folder does not exist
TEST_F(CmdLineBaseExecutorTests, RunTest_InputFolderNotExist)
{
    // Arrange
    CLTContext context;
    FvaConfiguration cfg;
    MockCmdLineBaseExecutor executor;
    MockCmdLineBaseTask mockTask;

    // Act
    FVA_EXIT_CODE result = executor.run(context, cfg);

    // Assert
    EXPECT_EQ(result, FVA_ERROR_INPUT_DIR_NOT_EXIST_ARG);
}

// Test case for run() when command is unknown
TEST_F(CmdLineBaseExecutorTests, RunTest_UnknownCommand)
{
    // Arrange
    CLTContext context;
    context.dir = "D:/a/fva/fva/build"; // Set the directory to a test value, somewhere on your system
    FvaConfiguration cfg;
    MockCmdLineBaseExecutor executor;
    MockCmdLineBaseTask mockTask;

    // Set up the context with an unknown command
    context.cmdType = "unknown_command";

    // Act
    FVA_EXIT_CODE result = executor.run(context, cfg);

    // Assert
    EXPECT_EQ(result, FVA_ERROR_UKNOWN_CMD);
}

// Test case for run() when command does not support readonly mode
TEST_F(CmdLineBaseExecutorTests, RunTest_CommandDoesNotSupportReadOnly)
{
    // Arrange
    CLTContext context;
    context.dir = "D:/a/fva/fva/build"; // Set the directory to a test value, somewhere on your system
    FvaConfiguration cfg;
    MockCmdLineBaseExecutor executor;
    MockCmdLineBaseTask mockTask;

    // Set up the context with readonly mode enabled
    context.readOnly = true;

    // Act
    FVA_EXIT_CODE result = executor.run(context, cfg);

    // Assert
    EXPECT_EQ(result, FVA_ERROR_NOT_SUPPORTED_RO_MODE);
}

// Test case for run() when command is executed in recursive mode
TEST_F(CmdLineBaseExecutorTests, RunTest_RecursiveMode)
{
    // Arrange
    CLTContext context;
    context.dir = "D:/a/fva/fva/build"; // Set the directory to a test value, somewhere on your system
    FvaConfiguration cfg;
    MockCmdLineBaseExecutor executor;
    MockCmdLineBaseTask mockTask;

    // Set up the context with recursive mode enabled
    context.recursive = true;

    // Act
    FVA_EXIT_CODE result = executor.run(context, cfg);

    // Assert
    EXPECT_EQ(result, 0);
}

// Test case for run() when command is executed in non-recursive mode
TEST_F(CmdLineBaseExecutorTests, RunTest_NonRecursiveMode)
{
    // Arrange
    CLTContext context;
    context.dir = "D:/a/fva/fva/build"; // Set the directory to a test value, somewhere on your system
    FvaConfiguration cfg;
    MockCmdLineBaseExecutor executor;
    MockCmdLineBaseTask mockTask;

    // Act
    FVA_EXIT_CODE result = executor.run(context, cfg);

    // Assert
    EXPECT_EQ(result, 0);
}
