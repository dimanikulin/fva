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

    cfg.setParam("Format::fvaDirName", "fvaDir");
    cfg.setParam("Format::fvaDirNameYear", "fvaDirYear");
    cfg.setParam("Format::fvaFileName", "fvaFile");
    cfg.setParam("Format::exifDateTime", "exifDateTime");
    cfg.setParam("Format::fileName1", "file1");
    cfg.setParam("Format::fileName2", "file2");
    cfg.setParam("Format::riffDateTime1", "riffDateTime1");
    cfg.setParam("Format::riffDateTime2", "riffDateTime2");

    // Act
    FVA_EXIT_CODE result = fmtContext.fillFmtContextFromCfg(cfg);

    // Assert
    /*EXPECT_EQ(result, FVA_NO_ERROR);
    EXPECT_EQ(fmtContext.fvaDirName, "fvaDir");
    EXPECT_EQ(fmtContext.fvaDirNameYear, "fvaDirYear");
    EXPECT_EQ(fmtContext.fvaFileName, "fvaFile");
    EXPECT_EQ(fmtContext.exifDateTime, "exifDateTime");
    EXPECT_EQ(fmtContext.fileName1, "file1");
    EXPECT_EQ(fmtContext.fileName2, "file2");
    EXPECT_EQ(fmtContext.riffDateTime1, "riffDateTime1");
    EXPECT_EQ(fmtContext.riffDateTime2, "riffDateTime2");*/
}

// Test case for fillFmtContextFromCfg when some parameters are missing
TEST_F(FvaFmtContextTests, FillFmtContextFromCfg_SomeParametersMissing)
{
    // Arrange
    FvaConfiguration cfg;
    FvaFmtContext fmtContext;

    cfg.setParam("Format::fvaDirName", "fvaDir");
    cfg.setParam("Format::fvaDirNameYear", "fvaDirYear");
    cfg.setParam("Format::fvaFileName", "fvaFile");
    cfg.setParam("Format::exifDateTime", "exifDateTime");

    // Act
    FVA_EXIT_CODE result = fmtContext.fillFmtContextFromCfg(cfg);

    // Assert
    /*EXPECT_EQ(result, FVA_ERROR_CANT_GET_PARAM);
    // Check that the parameters that were set are correctly filled in the fmtContext object
    EXPECT_EQ(fmtContext.fvaDirName, "fvaDir");
    EXPECT_EQ(fmtContext.fvaDirNameYear, "fvaDirYear");
    EXPECT_EQ(fmtContext.fvaFileName, "fvaFile");
    EXPECT_EQ(fmtContext.exifDateTime, "exifDateTime");
    // Check that the remaining parameters are empty strings
    EXPECT_EQ(fmtContext.fileName1, "");
    EXPECT_EQ(fmtContext.fileName2, "");
    EXPECT_EQ(fmtContext.riffDateTime1, "");
    EXPECT_EQ(fmtContext.riffDateTime2, "");*/
}

// TODO Add more test cases as needed for different scenarios and edge cases