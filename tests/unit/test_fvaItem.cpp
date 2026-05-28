#include <gtest/gtest.h>

#include <any>
#include <chrono>
#include <iostream>
#include <map>
#include <string>

#include "fvaFile.h"
#include "fvaFolder.h"
#include "fvaItem.h"

static std::chrono::system_clock::time_point makeDate(int year, int month, int day) {
    std::tm t{};
    t.tm_year = year - 1900;
    t.tm_mon = month - 1;
    t.tm_mday = day;
    return std::chrono::system_clock::from_time_t(std::mktime(&t));
}

// Test case for FVA_FS_TYPE_FILE
TEST(FvaItemTests, GetGuiName_File) {
    // Arrange
    fvaItem item;
    item.type = FVA_FS_TYPE_IMG;
    item.dateFrom = makeDate(2022, 1, 1);

    std::map<std::string, std::any> dictionaries;

    // Act
    std::string guiName = item.getGuiName(dictionaries);

    // Assert
    EXPECT_EQ(guiName, "2022-01-01 (00:00:00)");
}

// Test case for FVA_FS_TYPE_DIR with valid dateTo and without pFvaFolder
TEST(FvaItemTests, GetGuiName_Directory_ValidDateToWithoutFolder) {
    // Arrange
    fvaItem item;
    item.type = FVA_FS_TYPE_DIR;
    item.dateFrom = makeDate(2022, 1, 1);
    item.dateTo = makeDate(2022, 1, 31);

    std::map<std::string, std::any> dictionaries;

    // Act
    std::string guiName = item.getGuiName(dictionaries);

    // Assert
    EXPECT_EQ(guiName, "2022/01/01-2022/01/31");
}

// Test case for FVA_FS_TYPE_DIR without valid dateTo
TEST(FvaItemTests, GetGuiName_Directory_WithoutValidDateTo) {
    // Arrange
    fvaItem item;
    item.type = FVA_FS_TYPE_DIR;
    item.dateFrom = makeDate(2022, 1, 1);

    std::map<std::string, std::any> dictionaries;

    // Act
    std::string guiName = item.getGuiName(dictionaries);

    // Assert
    EXPECT_EQ(guiName, "2022/01/01");
}

// Test case for getGuiFullName with empty dictionaries
TEST(FvaItemTests, GetGuiFullName_EmptyDictionaries) {
    // Arrange
    fvaItem item;
    item.type = FVA_FS_TYPE_IMG;
    item.dateFrom = makeDate(2022, 1, 1);
    item.dateTo = makeDate(2022, 1, 2);
    item.fsFullPath = "/path/to/file.txt";
    item.isFiltered = false;
    item.pFvaFile = new fvaFile();

    std::map<std::string, std::any> dictionaries;

    // Act
    std::string guiFullName = item.getGuiFullName(dictionaries);

    // Assert
    ASSERT_STREQ(guiFullName.c_str(), "");
}

// Test case for getGuiName with empty dictionaries
TEST(FvaItemTests, GetGuiName_EmptyDictionaries) {
    // Arrange
    fvaItem item;
    item.type = FVA_FS_TYPE_DIR;
    item.dateFrom = makeDate(2022, 1, 1);
    item.dateTo = makeDate(2022, 1, 31);
    item.fsFullPath = "/path/to/folder";
    item.isFiltered = false;
    item.pFvaFolder = new fvaFolder();

    std::map<std::string, std::any> dictionaries;

    // Act
    std::string guiName = item.getGuiName(dictionaries);

    // Assert
    ASSERT_STREQ(guiName.c_str(), "2022/01/01-2022/01/31");
}
