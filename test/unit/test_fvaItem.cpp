#include <gtest/gtest.h>
#include "fvaItem.h"
#include "fvaFolder.h"
#include "fvaFile.h"

#include <iostream>


// Test case for FVA_FS_TYPE_DIR with valid dateTo and pFvaFolder
TEST(FvaItemTests, GetGuiName_Directory_ValidDateToAndFolder)
{
    // Arrange
    fvaItem item;
    item.type = FVA_FS_TYPE_DIR;
    item.dateFrom = QDateTime::fromString("2022/01/01" , "yyyy/MM/dd");
    item.dateTo = QDateTime::fromString("2022/01/31" , "yyyy/MM/dd");
    item.pFvaFolder = new fvaFolder;
    item.pFvaFolder->eventId = 1;
    item.pFvaFolder->eventReasonPeopleIds.append(2);

    QVariantMap dictionaries;
    QVariantMap event;
    event["ID"] = 1;
    event["fullName"] = "Event 1";
    QVariantMap person;
    person["ID"] = 2;
    person["name"] = "Person 1";
    QVariantList eventsList;
    eventsList.append(event);
    QVariantList peopleList;
    peopleList.append(person);
    dictionaries["events"] = eventsList;
    dictionaries["people"] = peopleList;

    // Act
    QString guiName = item.getGuiName(dictionaries);

    // std::cout << "guiName - " << guiName.toStdString() << std::endl;

    // Assert
    EXPECT_EQ(guiName, "2022/01/01-2022/01/31 - Event 1,Person 1");

    // Clean up
    delete item.pFvaFolder;
}
/*
// Test case for FVA_FS_TYPE_DIR with valid dateTo and pFvaFolder without eventReasonPeopleIds
TEST(FvaItemTests, GetGuiName_Directory_ValidDateToAndFolderWithoutEventReasonPeopleIds)
{
    // Arrange
    fvaItem item;
    item.type = FVA_FS_TYPE_DIR;
    item.dateFrom = QDate(2022, 1, 1);
    item.dateTo = QDate(2022, 1, 31);
    item.pFvaFolder = new fvaFolder;
    item.pFvaFolder->eventId = 1;

    QVariantMap dictionaries;
    QVariantMap event;
    event["ID"] = 1;
    event["fullName"] = "Event 1";
    QVariantList eventsList;
    eventsList.append(event);
    dictionaries["events"] = eventsList;

    // Act
    QString guiName = item.getGuiName(dictionaries);

    // Assert
    EXPECT_EQ(guiName, "2022/01/01-2022/01/31 - Event 1");

    // Clean up
    delete item.pFvaFolder;
}

// Test case for FVA_FS_TYPE_DIR with valid dateTo and pFvaFolder with invalid eventId
TEST(FvaItemTests, GetGuiName_Directory_ValidDateToAndFolderWithInvalidEventId)
{
    // Arrange
    fvaItem item;
    item.type = FVA_FS_TYPE_DIR;
    item.dateFrom = QDate(2022, 1, 1);
    item.dateTo = QDate(2022, 1, 31);
    item.pFvaFolder = new FvaFolder();
    item.pFvaFolder->eventId = 2;

    QVariantMap dictionaries;
    QVariantMap event;
    event["ID"] = 1;
    event["fullName"] = "Event 1";
    QVariantList eventsList;
    eventsList.append(event);
    dictionaries["events"] = eventsList;

    // Act
    QString guiName = item.getGuiName(dictionaries);

    // Assert
    EXPECT_EQ(guiName, "2022/01/01-2022/01/31");

    // Clean up
    delete item.pFvaFolder;
}

// Test case for FVA_FS_TYPE_DIR with valid dateTo and pFvaFolder with invalid eventReasonPeopleIds
TEST(FvaItemTests, GetGuiName_Directory_ValidDateToAndFolderWithInvalidEventReasonPeopleIds)
{
    // Arrange
    fvaItem item;
    item.type = FVA_FS_TYPE_DIR;
    item.dateFrom = QDate(2022, 1, 1);
    item.dateTo = QDate(2022, 1, 31);
    item.pFvaFolder = new FvaFolder();
    item.pFvaFolder->eventId = 1;
    item.pFvaFolder->eventReasonPeopleIds.append(3);

    QVariantMap dictionaries;
    QVariantMap event;
    event["ID"] = 1;
    event["fullName"] = "Event 1";
    QVariantMap person;
    person["ID"] = 2;
    person["name"] = "Person 1";
    QVariantList eventsList;
    eventsList.append(event);
    QVariantList peopleList;
    peopleList.append(person);
    dictionaries["events"] = eventsList;
    dictionaries["people"] = peopleList;

    // Act
    QString guiName = item.getGuiName(dictionaries);

    // Assert
    EXPECT_EQ(guiName, "2022/01/01-2022/01/31 - Event 1");

    // Clean up
    delete item.pFvaFolder;
}

// Test case for FVA_FS_TYPE_DIR with valid dateTo and without pFvaFolder
TEST(FvaItemTests, GetGuiName_Directory_ValidDateToWithoutFolder)
{
    // Arrange
    fvaItem item;
    item.type = FVA_FS_TYPE_DIR;
    item.dateFrom = QDate(2022, 1, 1);
    item.dateTo = QDate(2022, 1, 31);

    QVariantMap dictionaries;

    // Act
    QString guiName = item.getGuiName(dictionaries);

    // Assert
    EXPECT_EQ(guiName, "2022/01/01-2022/01/31");

    // Clean up
    delete item.pFvaFolder;
}

// Test case for FVA_FS_TYPE_DIR without valid dateTo
TEST(FvaItemTests, GetGuiName_Directory_WithoutValidDateTo)
{
    // Arrange
    fvaItem item;
    item.type = FVA_FS_TYPE_DIR;
    item.dateFrom = QDate(2022, 1, 1);

    QVariantMap dictionaries;

    // Act
    QString guiName = item.getGuiName(dictionaries);

    // Assert
    EXPECT_EQ(guiName, "2022/01/01");

    // Clean up
    delete item.pFvaFolder;
}

// Test case for FVA_FS_TYPE_FILE
TEST(FvaItemTests, GetGuiName_File)
{
    // Arrange
    fvaItem item;
    item.type = FVA_FS_TYPE_FILE;
    item.dateFrom = QDate(2022, 1, 1);

    QVariantMap dictionaries;

    // Act
    QString guiName = item.getGuiName(dictionaries);

    // Assert
    EXPECT_EQ(guiName, "2022-01-01 (00:00:00)");

    // Clean up
    delete item.pFvaFolder;
}

// Test case for getGuiName with valid dictionaries
TEST(FvaItemTests, GetGuiName_ValidDictionaries)
{
    // Arrange
    fvaItem item;
    item.type = FVA_FS_TYPE_FILE;
    item.dateFrom = QDateTime(QDate(2022, 1, 1));
    item.dateTo = QDateTime(QDate(2022, 1, 31));
    item.fsFullPath = "/path/to/file.txt";
    item.isFiltered = true;
    item.pFvaFile = new fvaFile();

    QVariantMap dictionaries;
    QVariantMap file;
    file["ID"] = 1;
    file["name"] = "File 1";
    QVariantList fileList;
    fileList.append(file);
    dictionaries["files"] = fileList;

    // Act
    QString guiName = item.getGuiName(dictionaries);

    // Assert
    EXPECT_EQ(guiName, "2022-01-01 (00:00:00) - File 1");

    // Clean up
    delete item.pFvaFile;
}

// Test case for getGuiName with empty dictionaries
TEST(FvaItemTests, GetGuiName_EmptyDictionaries)
{
    // Arrange
    fvaItem item;
    item.type = FVA_FS_TYPE_DIR;
    item.dateFrom = QDateTime(QDate(2022, 1, 1));
    item.dateTo = QDateTime(QDate(2022, 1, 31));
    item.fsFullPath = "/path/to/folder";
    item.isFiltered = false;
    item.pFvaFolder = new fvaFolder();

    QVariantMap dictionaries;

    // Act
    QString guiName = item.getGuiName(dictionaries);

    // Assert
    EXPECT_EQ(guiName, "2022/01/01-2022/01/31");

    // Clean up
    delete item.pFvaFolder;
}


// Test case for getGuiFullName with valid dictionaries
TEST(FvaItemTests, GetGuiFullName_ValidDictionaries)
{
    // Arrange
    fvaItem item;
    item.type = FVA_FS_TYPE_DIR;
    item.dateFrom = QDateTime(QDate(2022, 1, 1));
    item.dateTo = QDateTime(QDate(2022, 1, 31));
    item.fsFullPath = "/path/to/folder";
    item.isFiltered = true;
    item.pFvaFolder = new fvaFolder();

    QVariantMap dictionaries;
    QVariantMap folder;
    folder["ID"] = 1;
    folder["name"] = "Folder 1";
    QVariantList folderList;
    folderList.append(folder);
    dictionaries["folders"] = folderList;

    // Act
    QString guiFullName = item.getGuiFullName(dictionaries);

    // Assert
    EXPECT_EQ(guiFullName, "2022/01/01-2022/01/31 - Folder 1");

    // Clean up
    delete item.pFvaFolder;
}

// Test case for getGuiFullName with empty dictionaries
TEST(FvaItemTests, GetGuiFullName_EmptyDictionaries)
{
    // Arrange
    fvaItem item;
    item.type = FVA_FS_TYPE_FILE;
    item.dateFrom = QDateTime(QDate(2022, 1, 1));
    item.dateTo = QDateTime(QDate(2022, 1, 31));
    item.fsFullPath = "/path/to/file.txt";
    item.isFiltered = false;
    item.pFvaFile = new fvaFile();

    QVariantMap dictionaries;

    // Act
    QString guiFullName = item.getGuiFullName(dictionaries);

    // Assert
    EXPECT_EQ(guiFullName, "2022-01-01 (00:00:00)");

    // Clean up
    delete item.pFvaFile;
}
*/