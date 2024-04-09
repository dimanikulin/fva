// Test case for processFolderRecursivly when folder contains no subfolders or files
TEST(CmdLineBaseTaskTests, ProcessFolderRecursivly_NoSubfoldersOrFiles)
{
    // Arrange
    QString folder = "/path/to/folder";
    CLTContext context;
    CmdLineBaseTask task;

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
    CmdLineBaseTask task;

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
    CmdLineBaseTask task;

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
    CmdLineBaseTask task;

    // Mock the execute function to return an error code
    task.execute = [&](const CLTContext&) {
        return FVA_ERROR;
    };

    // Act
    FVA_EXIT_CODE result = task.processFolderRecursivly(folder, context);

    // Assert
    EXPECT_EQ(result, FVA_ERROR);
}