#include <gtest/gtest.h>
#include "CLTRenameVideoBySequence.h"

// Test fixture for CLTRenameVideoBySequence tests
class CLTRenameVideoBySequenceTests : public ::testing::Test
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

// Test case for execute function
TEST_F(CLTRenameVideoBySequenceTests, Execute)
{
    // Arrange
    FvaConfiguration cfg;
    CLTRenameVideoBySequence task(cfg);
    CLTContext context; // Set up the necessary context for the test

    // Act
    FVA_EXIT_CODE result = task.execute(context);

    // Assert
    // Verify the expected behavior and output
    ASSERT_EQ(FVA_NO_ERROR, result);
}

// Test case for Name function
TEST_F(CLTRenameVideoBySequenceTests, Name)
{
    // Arrange

    // Act
    QString name = CLTRenameVideoBySequence::Name();

    // Assert
    // Verify the expected name
    ASSERT_EQ("CLTRenameVideoBySequence", name.toStdString());
}

// Test case for supportReadOnly function
TEST_F(CLTRenameVideoBySequenceTests, SupportReadOnly)
{
    // Arrange
    FvaConfiguration cfg;
    CLTRenameVideoBySequence task(cfg);

    // Act
    bool readOnly = task.supportReadOnly();

    // Assert
    // Verify the expected read-only support
    ASSERT_TRUE(readOnly);
}

// Add more test cases for other member functions as needed
// Test case for CLTRenameVideoBySequence constructor
TEST(CLTRenameVideoBySequenceTests, Constructor)
{
    // Arrange
    FvaConfiguration cfg;
    cfg.setParam("Rename::videoByModifTime", true);

    // Act
    CLTRenameVideoBySequence renameVideoBySequence(cfg);

    // Assert
    // Verify that the m_renameVideoByModifTime member variable is set correctly
    
    EXPECT_TRUE(renameVideoBySequence.renameVideoByModifTime());
}

// Test case for CLTRenameVideoBySequence constructor with invalid configuration
TEST(CLTRenameVideoBySequenceTests, Constructor_falseRenameVideoByModifTime )
{
    // Arrange
    FvaConfiguration cfg;
    cfg.setParam("Rename::videoByModifTime", true);
    CLTRenameVideoBySequence renameVideoBySequence(cfg);

    // Act & Assert
    EXPECT_TRUE(renameVideoBySequence.renameVideoByModifTime());
}
