#include <gtest/gtest.h>
#include "fvacommoncsv.h"
#include "mocks.h"

#include <iostream>
#include <filesystem>
#include <fstream>

namespace fs = std::filesystem;

// Test fixture for fvaBaseDictionaryItem tests
class fvacommoncsvTests : public ::testing::Test
{
protected:
    void SetUp() override
    {
        fs::path current_path = fs::current_path();
        std::cout << "Current working directory: " << current_path << '\n';
    }

    void TearDown() override
    {
        // Clean up any resources used by the tests
    }
};

// Test case for loading a simple map from CSV with a specific item type filter
TEST_F(fvacommoncsvTests, LoadMapWithItemTypeFilter)
{
    // Arrange
    QString rootSWdir = TEST_DIR + "/test_data/";
    FVA_SIMPLE_MAP simpleMap;
    QString dictName = "dictionary.csv";
    int typeToFilter = 1;

    // Act
    FVA_EXIT_CODE result = fvaLoadSimpleMapFromCsvByItemType(rootSWdir, simpleMap, dictName, typeToFilter);

    // Assert
    EXPECT_EQ(result, FVA_NO_ERROR);
    // Add additional assertions to verify the contents of the simpleMap
}

// Test case for loading a simple map from CSV without any item type filter
TEST_F(fvacommoncsvTests, LoadMapWithoutItemTypeFilter)
{
    // Arrange
    QString rootSWdir = TEST_DIR +"/test_data/";
    FVA_SIMPLE_MAP simpleMap;
    QString dictName = "dictionary.csv";

    // Act
    FVA_EXIT_CODE result = fvaLoadSimpleMapFromCsvByItemType(rootSWdir, simpleMap, dictName);

    // Assert
    EXPECT_EQ(result, FVA_NO_ERROR);
    // Add additional assertions to verify the contents of the simpleMap
}

// Test case for loading a simple map from CSV with an invalid item type filter
TEST_F(fvacommoncsvTests, LoadMapWithInvalidItemTypeFilter)
{
    // Arrange
    QString rootSWdir = TEST_DIR + "/test_data/";
    FVA_SIMPLE_MAP simpleMap;
    QString dictName = "dictionary.csv";
    int typeToFilter = -1;

    // Act
    FVA_EXIT_CODE result = fvaLoadSimpleMapFromCsvByItemType(rootSWdir, simpleMap, dictName, typeToFilter);

    // Assert
    EXPECT_EQ(result, FVA_ERROR_WRONG_PARAMETERS);
    // Add additional assertions to verify the contents of the simpleMap (should be empty)
}

// Test case for loading a valid CSV file
TEST_F(fvacommoncsvTests, LoadValidCsvFile)
{
    // Arrange
    QString rootSWdir = TEST_DIR + "/test_data/";
    QString fvaFileName = "fva.csv";
    FVA_FILE_INFO_MAP fvaFileInfo;

    // Act
    FVA_EXIT_CODE result = fvaLoadFvaFileInfoFromCsv(rootSWdir, fvaFileInfo, fvaFileName);

    // Assert
    EXPECT_EQ(result, FVA_NO_ERROR);
    // Add additional assertions to verify the loaded file information
}

// Test case for loading a CSV file with missing mandatory fields
TEST_F(fvacommoncsvTests, LoadCsvFileWithMissingFields)
{
    // Arrange
    QString rootSWdir = TEST_DIR + "/test_data/";
    QString fvaFileName = "fva_missing_fields.csv";
    FVA_FILE_INFO_MAP fvaFileInfo;

    // Act
    FVA_EXIT_CODE result = fvaLoadFvaFileInfoFromCsv(rootSWdir, fvaFileInfo, fvaFileName);

    // Assert
    EXPECT_EQ(result, FVA_ERROR_CANT_FIND_MANDATORY_FIELDS);
    // Add additional assertions to verify the error handling
}

// Test case for loading a CSV file with non-unique file names
TEST_F(fvacommoncsvTests, LoadCsvFileWithNonUniqueFileNames)
{
    // Arrange
    QString rootSWdir = TEST_DIR + "/test_data/";
    QString fvaFileName = "fva_non_unique_names.csv";
    FVA_FILE_INFO_MAP fvaFileInfo;

    // Act
    FVA_EXIT_CODE result = fvaLoadFvaFileInfoFromCsv(rootSWdir, fvaFileInfo, fvaFileName);
                                                
    // Assert
    EXPECT_EQ(result, FVA_ERROR_NON_UNIQUE_FVA_INFO);
    // Add additional assertions to verify the error handling
}

// Test case for loading a CSV file that does not exist
TEST_F(fvacommoncsvTests, LoadNonExistentCsvFile)
{
    // Arrange
    QString rootSWdir = TEST_DIR + "/test_data/";
    QString fvaFileName = "non_existent_file.csv";
    FVA_FILE_INFO_MAP fvaFileInfo;

    // Act
    FVA_EXIT_CODE result = fvaLoadFvaFileInfoFromCsv(rootSWdir, fvaFileInfo, fvaFileName);

    // Assert
    EXPECT_EQ(result, FVA_ERROR_CANT_OPEN_INPUT_FILE);
    // Add additional assertions to verify the error handling
}
