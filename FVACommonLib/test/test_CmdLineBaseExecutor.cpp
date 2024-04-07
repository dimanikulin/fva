// Test case for run() when input folder does not exist
TEST_F(CmdLineBaseExecutorTests, RunTest_InputFolderNotExist)
{
    // Arrange
    CLTContext context;
    FvaConfiguration cfg;
    CmdLineBaseExecutor executor;
    MockCmdLineBaseTask mockTask;

    // Set up expectations on the mock task
    EXPECT_CALL(mockTask, execute()).Times(0); // The task should not be executed

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
    FvaConfiguration cfg;
    CmdLineBaseExecutor executor;
    MockCmdLineBaseTask mockTask;

    // Set up expectations on the mock task
    EXPECT_CALL(mockTask, execute()).Times(0); // The task should not be executed

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
    FvaConfiguration cfg;
    CmdLineBaseExecutor executor;
    MockCmdLineBaseTask mockTask;

    // Set up expectations on the mock task
    EXPECT_CALL(mockTask, execute()).Times(0); // The task should not be executed

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
    FvaConfiguration cfg;
    CmdLineBaseExecutor executor;
    MockCmdLineBaseTask mockTask;

    // Set up expectations on the mock task
    EXPECT_CALL(mockTask, processFolderRecursivly(context.dir, context)).Times(1);

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
    FvaConfiguration cfg;
    CmdLineBaseExecutor executor;
    MockCmdLineBaseTask mockTask;

    // Set up expectations on the mock task
    EXPECT_CALL(mockTask, execute(context)).Times(1);

    // Act
    FVA_EXIT_CODE result = executor.run(context, cfg);

    // Assert
    EXPECT_EQ(result, 0);
}