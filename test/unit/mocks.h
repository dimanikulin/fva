#ifndef UNIT_MOCKS_H
#define UNIT_MOCKS_H

#include <gmock/gmock.h>
#include "CmdLineBaseExecutor.h"
#include "FVADataProcessor.h"
#include "CmdLineBaseTask.h"
#include "FvaConfiguration.h"
#include "fvacltcontext.h"

const char[] TEST_DIR = "D:/a/fva/fva/build"; // Set the directory to a test value, somewhere on your system

// Mock implementation of CmdLineBaseTask for testing purposes
class MockCmdLineBaseTask : public CmdLineBaseTask
{
public:
    MOCK_METHOD(FVA_EXIT_CODE, execute, (const CLTContext&), (override));
};

// Mock implementation of CmdLineBaseExecutor for testing purposes
class MockCmdLineBaseExecutor : public CmdLineBaseExecutor
{
public:
    std::unique_ptr<CmdLineBaseTask> createTaskByName(const CLTContext& context, const FvaConfiguration& cfg) override
    {
        return std::make_unique<MockCmdLineBaseTask>();
    }
};
// Mock implementation of CmdLineBaseExecutor for testing purposes
class MockCmdLineBaseExecutorEmptyCmd : public CmdLineBaseExecutor
{
public:
    std::unique_ptr<CmdLineBaseTask> createTaskByName(const CLTContext& context, const FvaConfiguration& cfg) override
    {
        return std::unique_ptr<CmdLineBaseTask>(); // Return null to simulate an unknown command
    }
};
#endif // UNIT_MOCKS_H