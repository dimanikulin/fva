// Test case for loading a valid CSV file
TEST(FvaLoadFvaFileInfoFromCsvTests, LoadValidCsvFile)
{
    // Arrange
    QString rootSWdir = "/path/to/rootSWdir/";
    QString fvaFileName = "fva.csv";
    FVA_FILE_INFO_MAP fvaFileInfo;

    // Act
    FVA_EXIT_CODE result = fvaLoadFvaFileInfoFromCsv(rootSWdir, fvaFileInfo, fvaFileName);

    // Assert
    EXPECT_EQ(result, FVA_NO_ERROR);
    // Add additional assertions to verify the loaded file information
}

// Test case for loading a CSV file with missing mandatory fields
TEST(FvaLoadFvaFileInfoFromCsvTests, LoadCsvFileWithMissingFields)
{
    // Arrange
    QString rootSWdir = "/path/to/rootSWdir/";
    QString fvaFileName = "fva_missing_fields.csv";
    FVA_FILE_INFO_MAP fvaFileInfo;

    // Act
    FVA_EXIT_CODE result = fvaLoadFvaFileInfoFromCsv(rootSWdir, fvaFileInfo, fvaFileName);

    // Assert
    EXPECT_EQ(result, FVA_ERROR_CANT_FIND_MANDATORY_FIELDS);
    // Add additional assertions to verify the error handling
}

// Test case for loading a CSV file with non-unique file names
TEST(FvaLoadFvaFileInfoFromCsvTests, LoadCsvFileWithNonUniqueFileNames)
{
    // Arrange
    QString rootSWdir = "/path/to/rootSWdir/";
    QString fvaFileName = "fva_non_unique_names.csv";
    FVA_FILE_INFO_MAP fvaFileInfo;

    // Act
    FVA_EXIT_CODE result = fvaLoadFvaFileInfoFromCsv(rootSWdir, fvaFileInfo, fvaFileName);

    // Assert
    EXPECT_EQ(result, FVA_ERROR_NON_UNIQUE_FVA_INFO);
    // Add additional assertions to verify the error handling
}

// Test case for loading a CSV file that does not exist
TEST(FvaLoadFvaFileInfoFromCsvTests, LoadNonExistentCsvFile)
{
    // Arrange
    QString rootSWdir = "/path/to/rootSWdir/";
    QString fvaFileName = "non_existent_file.csv";
    FVA_FILE_INFO_MAP fvaFileInfo;

    // Act
    FVA_EXIT_CODE result = fvaLoadFvaFileInfoFromCsv(rootSWdir, fvaFileInfo, fvaFileName);

    // Assert
    EXPECT_EQ(result, FVA_ERROR_FILE_NOT_FOUND);
    // Add additional assertions to verify the error handling
}