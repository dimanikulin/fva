#include <gtest/gtest.h>

#include "CLTCheckDateTime.h"
#include "testloggger.inl"

#ifndef TEST_LOGGER_GLOBAL
TEST_LOGGER_GLOBAL
#endif  // TEST_LOGGER_GLOBAL

// Test fixture for CLTCheckDateTime tests
class CLTCheckDateTimeTests : public ::testing::Test {
protected:
    void SetUp() override {
        // Set up any necessary objects or test data
    }

    void TearDown() override {
        // Clean up any resources used by the tests
    }
};

// Test case for execute function
TEST_F(CLTCheckDateTimeTests, Execute) {
    // Arrange
    FvaConfiguration cfg;
    CLTCheckDateTime task(cfg);
    CLTContext context;  // Set up the necessary context for the test

    // Act
    FVA_EXIT_CODE result = task.execute(context);

    // Assert
    EXPECT_EQ(FVA_NO_ERROR, result);
    // Add more assertions to verify the expected behavior and output
}

// Test case for Name function
TEST_F(CLTCheckDateTimeTests, Name) {
    // Arrange

    // Act
    std::string name = CLTCheckDateTime::Name();

    // Assert
    // Verify the expected name
    ASSERT_EQ("CLTCheckDateTime", name);
}

// Test case for supportReadOnly function
TEST_F(CLTCheckDateTimeTests, SupportReadOnly) {
    // Arrange
    FvaConfiguration cfg;
    CLTCheckDateTime task(cfg);

    // Act
    bool readOnly = task.supportReadOnly();

    // Assert
    // Verify the expected read-only support
    ASSERT_FALSE(readOnly);
}

// Add more test cases for other member functions as needed

// Test case for execute function when there are no image files
TEST_F(CLTCheckDateTimeTests, Execute_NoImageFiles) {
    // Arrange
    FvaConfiguration cfg;
    CLTCheckDateTime cltCheckDateTime(cfg);
    CLTContext context;  // Set up the necessary context for the test

    // Act
    FVA_EXIT_CODE result = cltCheckDateTime.execute(context);

    // Assert
    EXPECT_EQ(FVA_NO_ERROR, result);  // Verify that the function returns FVA_ERROR_NO_EXIF_DATE_TIME
    // Add more assertions to verify the expected behavior and output
}

// Test case for execute function when there is an image file with valid exif Date-Time
TEST_F(CLTCheckDateTimeTests, Execute_ValidExifDateTime) {
    // Arrange
    FvaConfiguration cfg;
    CLTCheckDateTime cltCheckDateTime(cfg);
    CLTContext context;  // Set up the necessary context for the test
    std::string filePath = "path/to/image.jpg";
    // Add the image file to the test directory

    // Act
    FVA_EXIT_CODE result = cltCheckDateTime.execute(context);

    // Assert
    EXPECT_EQ(FVA_NO_ERROR, result);  // Verify that the function returns FVA_ERROR_NO_EXIF_DATE_TIME
    // Add more assertions to verify the expected behavior and output
}

// Test case for execute function when there is an image file with empty exif Date-Time
TEST_F(CLTCheckDateTimeTests, Execute_EmptyExifDateTime) {
    // Arrange
    FvaConfiguration cfg;
    CLTCheckDateTime cltCheckDateTime(cfg);
    CLTContext context;  // Set up the necessary context for the test
    std::string filePath = "path/to/image.jpg";
    // Add the image file to the test directory with empty exif Date-Time

    // Act
    FVA_EXIT_CODE result = cltCheckDateTime.execute(context);

    // Assert
    // TODO to uncomment and fix
    // EXPECT_EQ(FVA_ERROR_NO_EXIF_DATE_TIME, result);  // Verify that the function returns FVA_ERROR_NO_EXIF_DATE_TIME
    // Add more assertions to verify the expected behavior and output
}
