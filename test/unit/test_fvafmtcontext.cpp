#include <gtest/gtest.h>
#include "FvaFmtContext.h"

// Test fixture for FvaFmtContext tests
class FvaFmtContextTests : public ::testing::Test
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

// Test case for fillFmtContextFromCfg when all parameters are valid
TEST_F(FvaFmtContextTests, FillFmtContextFromCfg_AllParametersValid)
{
    // Arrange
    FvaConfiguration cfg;
    FvaFmtContext fmtContext;

    cfg.setParamAsString("Format::fvaDirName", "fvaDir");
    cfg.setParamAsString("Format::fvaDirNameYear", "fvaDirYear");
    cfg.setParamAsString("Format::fvaFileName", "fvaFile");
    cfg.setParamAsString("Format::exifDateTime", "exifDateTime");
    cfg.setParamAsString("Format::fileName1", "file1");
    cfg.setParamAsString("Format::fileName2", "file2");
    cfg.setParamAsString("Format::riffDateTime1", "riffDateTime1");
    cfg.setParamAsString("Format::riffDateTime2", "riffDateTime2");

    // Act
    FVA_EXIT_CODE result = fmtContext.fillFmtContextFromCfg(cfg);

    // Assert
    EXPECT_EQ(result, FVA_EXIT_CODE_SUCCESS);
    EXPECT_EQ(fmtContext.fvaDirName, "fvaDir");
    EXPECT_EQ(fmtContext.fvaDirNameYear, "fvaDirYear");
    EXPECT_EQ(fmtContext.fvaFileName, "fvaFile");
    EXPECT_EQ(fmtContext.exifDateTime, "exifDateTime");
    EXPECT_EQ(fmtContext.fileName1, "file1");
    EXPECT_EQ(fmtContext.fileName2, "file2");
    EXPECT_EQ(fmtContext.riffDateTime1, "riffDateTime1");
    EXPECT_EQ(fmtContext.riffDateTime2, "riffDateTime2");
}

// Test case for fillFmtContextFromCfg when some parameters are missing
TEST_F(FvaFmtContextTests, FillFmtContextFromCfg_SomeParametersMissing)
{
    // Arrange
    FvaConfiguration cfg;
    FvaFmtContext fmtContext;

    cfg.setParamAsString("Format::fvaDirName", "fvaDir");
    cfg.setParamAsString("Format::fvaDirNameYear", "fvaDirYear");
    cfg.setParamAsString("Format::fvaFileName", "fvaFile");
    cfg.setParamAsString("Format::exifDateTime", "exifDateTime");

    // Act
    FVA_EXIT_CODE result = fmtContext.fillFmtContextFromCfg(cfg);

    // Assert
    EXPECT_EQ(result, FVA_ERROR_MISSING_PARAMETER);
    // Check that the parameters that were set are correctly filled in the fmtContext object
    EXPECT_EQ(fmtContext.fvaDirName, "fvaDir");
    EXPECT_EQ(fmtContext.fvaDirNameYear, "fvaDirYear");
    EXPECT_EQ(fmtContext.fvaFileName, "fvaFile");
    EXPECT_EQ(fmtContext.exifDateTime, "exifDateTime");
    // Check that the remaining parameters are empty strings
    EXPECT_EQ(fmtContext.fileName1, "");
    EXPECT_EQ(fmtContext.fileName2, "");
    EXPECT_EQ(fmtContext.riffDateTime1, "");
    EXPECT_EQ(fmtContext.riffDateTime2, "");
}

// TODO Add more test cases as needed for different scenarios and edge cases