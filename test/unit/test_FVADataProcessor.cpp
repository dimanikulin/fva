#include <gtest/gtest.h>
#include "FVADataProcessor.h"
#include "CLTCreateDirStructByDeviceName.h"

// Test fixture for FVADataProcessor tests
class FVADataProcessorTests : public ::testing::Test
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

// Test case for createTaskByName function
TEST_F(FVADataProcessorTests, CreateTaskByName)
{
    // Arrange
    FVADataProcessor fvaDataProcessor;
    CLTContext context; // Set up the necessary context for the test
    FvaConfiguration cfg; // Set up the necessary configuration for the test

    // Act
    std::unique_ptr<CmdLineBaseTask> task = fvaDataProcessor.createTaskByName(context, cfg);

    // Assert
    // Verify that the task is created correctly
    ASSERT_NE(nullptr, task);
    // Add more assertions to verify the expected behavior and output of the created task
}

// Add more test cases for other member functions as needed
// Test case for creating CLTCreateDirStructByDeviceName task
TEST(FVADataProcessorTests, CreateTaskByName_CLTCreateDirStructByDeviceName)
{
    // Arrange
    CLTContext context;
    context.cmdType = CLTCreateDirStructByDeviceName::Name();
    FvaConfiguration cfg;
    FVADataProcessor dataProcessor;

    // Act
    std::unique_ptr<CmdLineBaseTask> task = dataProcessor.createTaskByName(context, cfg);

    // Assert
    ASSERT_TRUE(dynamic_cast<CLTCreateDirStructByDeviceName*>(task.get()) != nullptr);
}

// Test case for creating CLTCheckDeviceName task
TEST(FVADataProcessorTests, CreateTaskByName_CLTCheckDeviceName)
{
    // Arrange
    CLTContext context;
    context.cmdType = CLTCheckDeviceName::Name();
    FvaConfiguration cfg;
    FVADataProcessor dataProcessor;

    // Act
    std::unique_ptr<CmdLineBaseTask> task = dataProcessor.createTaskByName(context, cfg);

    // Assert
    ASSERT_TRUE(dynamic_cast<CLTCheckDeviceName*>(task.get()) != nullptr);
}

// Test case for creating CLTRenameVideoBySequence task
TEST(FVADataProcessorTests, CreateTaskByName_CLTRenameVideoBySequence)
{
    // Arrange
    CLTContext context;
    context.cmdType = CLTRenameVideoBySequence::Name();
    FvaConfiguration cfg;
    FVADataProcessor dataProcessor;

    // Act
    std::unique_ptr<CmdLineBaseTask> task = dataProcessor.createTaskByName(context, cfg);

    // Assert
    ASSERT_TRUE(dynamic_cast<CLTRenameVideoBySequence*>(task.get()) != nullptr);
}

// Add more test cases for other task types as needed
