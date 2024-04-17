#include <gtest/gtest.h>
#include "../FVADescriptionFile.h"

// Test case for saving an empty description file
TEST(FVADescriptionFileTests, SaveEmptyFile)
{
    // Arrange
    FVADescriptionFile descriptionFile;
    QString path = "test_file.csv";
    QStringList titles;
    FVADescriptionFile::DESCRIPTIONS_MAP decsItems;

    // Act
    FVA_EXIT_CODE result = descriptionFile.save(path, titles, decsItems);

    // Assert
    EXPECT_EQ(result, FVA_NO_ERROR);
    // TODO: Add assertions to check if the file was saved correctly
}

// Test case for saving a description file with one item
TEST(FVADescriptionFileTests, SaveFileWithOneItem)
{
    // Arrange
    FVADescriptionFile descriptionFile;
    QString path = "test_file.csv";
    QStringList titles = { "Title1", "Title2", "Title3" };
    FVADescriptionFile::DESCRIPTIONS_MAP decsItems;
    QStringList itemValues = { "Value1", "Value2", "Value3" };
    decsItems["Item1"] = itemValues;

    // Act
    FVA_EXIT_CODE result = descriptionFile.save(path, titles, decsItems);

    // Assert
    EXPECT_EQ(result, FVA_NO_ERROR);
    // TODO: Add assertions to check if the file was saved correctly
}

// Test case for saving a description file with multiple items
TEST(FVADescriptionFileTests, SaveFileWithMultipleItems)
{
    // Arrange
    FVADescriptionFile descriptionFile;
    QString path = "test_file.csv";
    QStringList titles = { "Title1", "Title2", "Title3" };
    FVADescriptionFile::DESCRIPTIONS_MAP decsItems;
    QStringList item1Values = { "Value1", "Value2", "Value3" };
    QStringList item2Values = { "Value4", "Value5", "Value6" };
    decsItems["Item1"] = item1Values;
    decsItems["Item2"] = item2Values;

    // Act
    FVA_EXIT_CODE result = descriptionFile.save(path, titles, decsItems);

    // Assert
    EXPECT_EQ(result, FVA_NO_ERROR);
    // TODO: Add assertions to check if the file was saved correctly
}

// Test case for loading an empty description file
TEST(FVADescriptionFileTests, LoadEmptyFile)
{
    // Arrange
    FVADescriptionFile descriptionFile;
    QString path = "test_file.csv";
    QStringList titles;
    FVADescriptionFile::DESCRIPTIONS_MAP decsItems;

    // Act
    FVA_EXIT_CODE result = descriptionFile.load(path, titles, decsItems);

    // Assert
    EXPECT_EQ(result, FVA_NO_ERROR);
    EXPECT_TRUE(titles.isEmpty());
    EXPECT_TRUE(decsItems.isEmpty());
}

// Test case for loading a description file with one item
TEST(FVADescriptionFileTests, LoadFileWithOneItem)
{
    // Arrange
    FVADescriptionFile descriptionFile;
    QString path = "test_file.csv";
    QStringList titles = { "Title1", "Title2", "Title3" };
    FVADescriptionFile::DESCRIPTIONS_MAP decsItems;

    // Act
    FVA_EXIT_CODE result = descriptionFile.load(path, titles, decsItems);

    // Assert
    EXPECT_EQ(result, FVA_NO_ERROR);
    EXPECT_EQ(titles.size(), 3);
    EXPECT_EQ(decsItems.size(), 1);
    EXPECT_TRUE(decsItems.contains("Item1"));
    EXPECT_EQ(decsItems["Item1"].size(), 3);
}

// Test case for loading a description file with multiple items
TEST(FVADescriptionFileTests, LoadFileWithMultipleItems)
{
    // Arrange
    FVADescriptionFile descriptionFile;
    QString path = "test_file.csv";
    QStringList titles = { "Title1", "Title2", "Title3" };
    FVADescriptionFile::DESCRIPTIONS_MAP decsItems;

    // Act
    FVA_EXIT_CODE result = descriptionFile.load(path, titles, decsItems);

    // Assert
    EXPECT_EQ(result, FVA_NO_ERROR);
    EXPECT_EQ(titles.size(), 3);
    EXPECT_EQ(decsItems.size(), 2);
    EXPECT_TRUE(decsItems.contains("Item1"));
    EXPECT_EQ(decsItems["Item1"].size(), 3);
    EXPECT_TRUE(decsItems.contains("Item2"));
    EXPECT_EQ(decsItems["Item2"].size(), 3);
}