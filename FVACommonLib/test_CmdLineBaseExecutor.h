#include <gtest/gtest.h>
#include "CmdLineBaseExecutor.h"

// Mock implementation of CmdLineBaseTask for testing purposes
class MockCmdLineBaseTask : public CmdLineBaseTask
{
public:
    MOCK_METHOD(void, execute, (), (override));
};

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

    // Helper function to create a mock task
    std::unique_ptr<CmdLineBaseTask> createMockTask()
    {
        return std::make_unique<MockCmdLineBaseTask>();
    }

    // Helper function to create a test instance of CmdLineBaseExecutor
    std::unique_ptr<CmdLineBaseExecutor> createExecutor()
    {
        return std::make_unique<CmdLineBaseExecutor>();
    }
};

// Test case for the run() method
TEST_F(CmdLineBaseExecutorTests, RunTest)
{
    // Arrange
    CLTContext context;
    FvaConfiguration cfg;
    CmdLineBaseExecutor executor;
    MockCmdLineBaseTask mockTask;

    // Set up expectations on the mock task
    EXPECT_CALL(mockTask, execute()).Times(1);

    // Stub the createTaskByName() method to return the mock task
    executor.createTaskByName = [&](const CLTContext& context, const FvaConfiguration& cfg) {
        return createMockTask();
    };

    // Act
    FVA_EXIT_CODE result = executor.run(context, cfg);

    // Assert
    EXPECT_EQ(result, 0);
}

// Run all the tests
int main(int argc, char** argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}