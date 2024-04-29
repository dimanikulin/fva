#include <gtest/gtest.h>
// #include "../FVAPeopleListDlg.h"

// Test fixture for FVAPeopleListDlg tests
class FVAPeopleListDlgTests : public ::testing::Test
{
protected:
    void SetUp() override
    {
        // Set up any necessary objects or test data
    }

    void TearDown() override
    {
        // Clean up any resources used by the tests
    }
};

/*
// Test case for constructor
TEST_F(FVAPeopleListDlgTests, Constructor)
{
    // Arrange
    QVariantMap dictionaries;
    QString dictPath = "/path/to/dictionaries";

    // Act
    FVAPeopleListDlg dlg(dictionaries, dictPath);

    // Assert
    // Verify that the dictionaries and dictPath are correctly initialized
    ASSERT_EQ(dictionaries, dlg.getUI().m_dictionaries);
    ASSERT_EQ(dictPath, dlg.getUI().m_dictPath);
}

// Test case for updateGui function
TEST(FVAPeopleListDlgTests, UpdateGui)
{
    // Arrange
    QVariantMap dictionaries;
    dictionaries["people"] = QVariantList{
        QVariantMap{{"name", "John"}, {"ID", "1"}},
        QVariantMap{{"name", "Alice"}, {"ID", "2"}},
        QVariantMap{{"name", "Bob"}, {"ID", "3"}}
    };
    QString dictPath = "/path/to/dictionary";
    FVAPeopleListDlg dialog(dictionaries, dictPath);

    // Act
    dialog.updateGui();

    // Assert
    QList<QListWidgetItem*> items = dialog.ui.listWidget->findItems("", Qt::MatchContains);
    ASSERT_EQ(3, items.size());
    EXPECT_EQ("John", items[0]->text());
    EXPECT_EQ("1", items[0]->data(1).toString());
    EXPECT_EQ("Alice", items[1]->text());
    EXPECT_EQ("2", items[1]->data(1).toString());
    EXPECT_EQ("Bob", items[2]->text());
    EXPECT_EQ("3", items[2]->data(1).toString());
}

// Test case for OnAddPeopleBtnPressed function
TEST(FVAPeopleListDlgTests, OnAddPeopleBtnPressed)
{
    // Arrange
    QVariantMap dictionaries;
    QString dictPath = "/path/to/dictionary";
    FVAPeopleListDlg dialog(dictionaries, dictPath);

    // Act
    dialog.OnAddPeopleBtnPressed();

    // Assert
    // Verify the expected behavior and output
    // Add assertions based on the behavior of the OnAddPeopleBtnPressed function
    // For example, you can check if the FVADictionaryEditor.exe process is started with the correct parameters
    // You can also check if the updateGui function is called after the process finishes
}
*/
// Add more test cases for other member functions as needed