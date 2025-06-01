#include <gtest/gtest.h>
#include "fvacommoncsv.h"

#include <iostream>
#include <filesystem>
#include <fstream>

void copy_folder(const fs::path& source, const fs::path& destination) {
    if (!fs::exists(source) || !fs::is_directory(source)) {
        throw std::runtime_error("Source folder does not exist or is not a directory.");
    }

    if (!fs::exists(destination)) {
        fs::create_directories(destination);
    }

    for (const auto& entry : fs::recursive_directory_iterator(source)) {
        const auto& path = entry.path();
        auto relative_path = fs::relative(path, source);
        auto dest_path = destination / relative_path;

        if (fs::is_directory(path)) {
            fs::create_directories(dest_path);
        } else if (fs::is_regular_file(path)) {
            fs::copy_file(path, dest_path, fs::copy_options::overwrite_existing);
        }
    }
}

// Test fixture for fvaBaseDictionaryItem tests
class FVABaseDictionaryItemTests : public ::testing::Test
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
TEST_F(fvaLoadSimpleMapFromCsvByItemTypeTest, LoadMapWithItemTypeFilter)
{
    // Arrange
    QString rootSWdir = "./test_data/";
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
TEST_F(fvaLoadSimpleMapFromCsvByItemTypeTest, LoadMapWithoutItemTypeFilter)
{
    // Arrange
    QString rootSWdir = "./test_data/";
    FVA_SIMPLE_MAP simpleMap;
    QString dictName = "dictionary.csv";

    // Act
    FVA_EXIT_CODE result = fvaLoadSimpleMapFromCsvByItemType(rootSWdir, simpleMap, dictName);

    // Assert
    EXPECT_EQ(result, FVA_NO_ERROR);
    // Add additional assertions to verify the contents of the simpleMap
}

// Test case for loading a simple map from CSV with an invalid item type filter
TEST_F(fvaLoadSimpleMapFromCsvByItemTypeTest, LoadMapWithInvalidItemTypeFilter)
{
    // Arrange
    QString rootSWdir = "./test_data/";
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
TEST_F(FvaLoadFvaFileInfoFromCsvTests, LoadValidCsvFile)
{
    // Arrange
    QString rootSWdir = "./test_data/";
    QString fvaFileName = "fva.csv";
    FVA_FILE_INFO_MAP fvaFileInfo;

    // Act
    FVA_EXIT_CODE result = fvaLoadFvaFileInfoFromCsv(rootSWdir, fvaFileInfo, fvaFileName);

    // Assert
    EXPECT_EQ(result, FVA_NO_ERROR);
    // Add additional assertions to verify the loaded file information
}

// Test case for loading a CSV file with missing mandatory fields
TEST_F(FvaLoadFvaFileInfoFromCsvTests, LoadCsvFileWithMissingFields)
{
    // Arrange
    QString rootSWdir = "./test_data/";
    QString fvaFileName = "fva_missing_fields.csv";
    FVA_FILE_INFO_MAP fvaFileInfo;

    // Act
    FVA_EXIT_CODE result = fvaLoadFvaFileInfoFromCsv(rootSWdir, fvaFileInfo, fvaFileName);

    // Assert
    EXPECT_EQ(result, FVA_ERROR_CANT_FIND_MANDATORY_FIELDS);
    // Add additional assertions to verify the error handling
}

// Test case for loading a CSV file with non-unique file names
TEST_F(FvaLoadFvaFileInfoFromCsvTests, LoadCsvFileWithNonUniqueFileNames)
{
    // Arrange
    QString rootSWdir = "./test_data/";
    QString fvaFileName = "fva_non_unique_names.csv";
    FVA_FILE_INFO_MAP fvaFileInfo;

    // Act
    FVA_EXIT_CODE result = fvaLoadFvaFileInfoFromCsv(rootSWdir, fvaFileInfo, fvaFileName);
                                                
    // Assert
    EXPECT_EQ(result, FVA_ERROR_NON_UNIQUE_FVA_INFO);
    // Add additional assertions to verify the error handling
}

// Test case for loading a CSV file that does not exist
TEST_F(FvaLoadFvaFileInfoFromCsvTests, LoadNonExistentCsvFile)
{
    // Arrange
    QString rootSWdir = "./test_data/";
    QString fvaFileName = "non_existent_file.csv";
    FVA_FILE_INFO_MAP fvaFileInfo;

    // Act
    FVA_EXIT_CODE result = fvaLoadFvaFileInfoFromCsv(rootSWdir, fvaFileInfo, fvaFileName);

    // Assert
    EXPECT_EQ(result, FVA_ERROR_CANT_OPEN_INPUT_FILE);
    // Add additional assertions to verify the error handling
}
