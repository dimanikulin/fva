#include <gtest/gtest.h>
#include "../FVAOrganizerDonePage.h"

// Test fixture for FVAOrganizerDonePage tests
class FVAOrganizerDonePageTests : public ::testing::Test
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

// Test case for constructor
TEST_F(FVAOrganizerDonePageTests, Constructor)
{
    // Arrange

    // Act
    FVAOrganizerDonePage donePage;

    // Assert
    // Verify the initial state of the page
    ASSERT_EQ("Last words", donePage.finishWords->text().toStdString());
}

// Add more test cases for other member functions as needed

// Test case for finishWords text
TEST(FVAOrganizerDonePageTests, FinishWordsText)
{
    // Arrange
    FVAOrganizerDonePage page;

    // Act
    QString text = page.finishWords->toPlainText();

    // Assert
    // Verify the expected text of the finishWords QTextBrowser
    // Add assertions to verify the content of the finishWords QTextBrowser
}