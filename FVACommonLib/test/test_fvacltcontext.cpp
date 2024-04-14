#include <gtest/gtest.h>
#include "../FVACommonLib/fvacltcontext.h"

// Test case for default constructor
TEST(CLTContextTests, DefaultConstructor)
{
    // Arrange
    CLTContext context;

    // Assert
    EXPECT_EQ(context.cmdType, "");
    EXPECT_EQ(context.dir, "");
    EXPECT_EQ(context.outputDir, "");
    EXPECT_FALSE(context.readOnly);
    EXPECT_EQ(context.custom, "");
    EXPECT_TRUE(context.recursive);
}

// Test case for setting values in the context
TEST(CLTContextTests, SetValues)
{
    // Arrange
    CLTContext context;

    // Act
    context.cmdType = "test_command";
    context.dir = "/path/to/work/folder";
    context.outputDir = "/path/to/output/folder";
    context.readOnly = true;
    context.custom = "custom_value";
    context.recursive = false;

    // Assert
    EXPECT_EQ(context.cmdType, "test_command");
    EXPECT_EQ(context.dir, "/path/to/work/folder");
    EXPECT_EQ(context.outputDir, "/path/to/output/folder");
    EXPECT_TRUE(context.readOnly);
    EXPECT_EQ(context.custom, "custom_value");
    EXPECT_FALSE(context.recursive);
}

// Run all the tests
int main(int argc, char** argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}