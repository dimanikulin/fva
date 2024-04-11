#include <gtest/gtest.h>
#include "../FVACommonLib/fvacommoncsv.h"

// Test case for loading a simple map from CSV with a specific item type filter
TEST(fvaLoadSimpleMapFromCsvByItemTypeTest, LoadMapWithItemTypeFilter)
{
    // Arrange
    QString rootSWdir = "/path/to/rootSWdir";
    FVA_SIMPLE_MAP simpleMap;
    QString dictName = "dictionary.csv";
    int typeToFilter = 1;

    // Act
    FVA_EXIT_CODE result = fvaLoadSimpleMapFromCsvByItemType(rootSWdir, simpleMap, dictName, typeToFilter);

    // Assert
    EXPECT_EQ(result, FVA_SUCCESS);
    // Add additional assertions to verify the contents of the simpleMap
}

// Test case for loading a simple map from CSV without any item type filter
TEST(fvaLoadSimpleMapFromCsvByItemTypeTest, LoadMapWithoutItemTypeFilter)
{
    // Arrange
    QString rootSWdir = "/path/to/rootSWdir";
    FVA_SIMPLE_MAP simpleMap;
    QString dictName = "dictionary.csv";

    // Act
    FVA_EXIT_CODE result = fvaLoadSimpleMapFromCsvByItemType(rootSWdir, simpleMap, dictName);

    // Assert
    EXPECT_EQ(result, FVA_SUCCESS);
    // Add additional assertions to verify the contents of the simpleMap
}

// Test case for loading a simple map from CSV with an invalid item type filter
TEST(fvaLoadSimpleMapFromCsvByItemTypeTest, LoadMapWithInvalidItemTypeFilter)
{
    // Arrange
    QString rootSWdir = "/path/to/rootSWdir";
    FVA_SIMPLE_MAP simpleMap;
    QString dictName = "dictionary.csv";
    int typeToFilter = -1;

    // Act
    FVA_EXIT_CODE result = fvaLoadSimpleMapFromCsvByItemType(rootSWdir, simpleMap, dictName, typeToFilter);

    // Assert
    EXPECT_EQ(result, FVA_INVALID_ARGUMENT);
    // Add additional assertions to verify the contents of the simpleMap (should be empty)
}

// Run all the tests
int main(int argc, char** argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}