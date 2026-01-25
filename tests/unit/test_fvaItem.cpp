#include <gtest/gtest.h>
#include "fvaItem.h"
#include "fvaFolder.h"
#include "fvaFile.h"

#include <iostream>


// Test case for FVA_FS_TYPE_FILE
TEST(FvaItemTests, GetGuiName_File)
{
    // Arrange
    fvaItem item;
    item.type = FVA_FS_TYPE_IMG;
    item.dateFrom = QDateTime::fromString("2022/01/01" , "yyyy/MM/dd");

    QVariantMap dictionaries;

    // Act
    QString guiName = item.getGuiName(dictionaries);

    // Assert
    EXPECT_EQ(guiName, "2022-01-01 (00:00:00)");
}

// Test case for FVA_FS_TYPE_DIR with valid dateTo and without pFvaFolder
TEST(FvaItemTests, GetGuiName_Directory_ValidDateToWithoutFolder)
{
    // Arrange
    fvaItem item;
    item.type = FVA_FS_TYPE_DIR;
    item.dateFrom = QDateTime::fromString("2022/01/01" , "yyyy/MM/dd");
    item.dateTo = QDateTime::fromString("2022/01/31" , "yyyy/MM/dd");

    QVariantMap dictionaries;

    // Act
    QString guiName = item.getGuiName(dictionaries);

    // Assert
    EXPECT_EQ(guiName, "2022/01/01-2022/01/31");
}

// Test case for FVA_FS_TYPE_DIR without valid dateTo
TEST(FvaItemTests, GetGuiName_Directory_WithoutValidDateTo)
{
    // Arrange
    fvaItem item;
    item.type = FVA_FS_TYPE_DIR;
    item.dateFrom = QDateTime::fromString("2022/01/01" , "yyyy/MM/dd");

    QVariantMap dictionaries;

    // Act
    QString guiName = item.getGuiName(dictionaries);

    // Assert
    EXPECT_EQ(guiName, "2022/01/01");
}

// Test case for getGuiFullName with empty dictionaries
TEST(FvaItemTests, GetGuiFullName_EmptyDictionaries)
{
    // Arrange
    fvaItem item;
    item.type = FVA_FS_TYPE_IMG;
    item.dateFrom = QDateTime::fromString("2022/01/01" , "yyyy/MM/dd");
    item.dateTo = QDateTime::fromString("2022/01/02" , "yyyy/MM/dd");
    item.fsFullPath = "/path/to/file.txt";
    item.isFiltered = false;
    item.pFvaFile = new fvaFile();

    QVariantMap dictionaries;

    // Act
    QString guiFullName = item.getGuiFullName(dictionaries);

    // Assert
    ASSERT_STREQ(guiFullName.toStdString().c_str(), "");
}

// Test case for getGuiName with empty dictionaries
TEST(FvaItemTests, GetGuiName_EmptyDictionaries)
{
    // Arrange
    fvaItem item;
    item.type = FVA_FS_TYPE_DIR;
    item.dateFrom = QDateTime::fromString("2022/01/01" , "yyyy/MM/dd");
    item.dateTo = QDateTime::fromString("2022/01/31" , "yyyy/MM/dd");
    item.fsFullPath = "/path/to/folder";
    item.isFiltered = false;
    item.pFvaFolder = new fvaFolder();

    QVariantMap dictionaries;

    // Act
    QString guiName = item.getGuiName(dictionaries);

    // Assert
    ASSERT_STREQ(guiName.toStdString().c_str(), "2022/01/01-2022/01/31");
}
