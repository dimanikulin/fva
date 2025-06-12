#include <gtest/gtest.h>
#include "fvacommonlib.h"


/*
17,Format::fvaDirName,yyyy.MM.dd
18,Format::fvaDirNameYear,yyyy
19,Format::fvaFileName,yyyy-MM-dd-hh-mm-ss
20,Format::exifDateTime,yyyy:MM:dd hh:mm:ss
21,Format::fileName1,yyyyMMdd_hhmmss
22,Format::fileName2,yyyyMMdd_hh_mm_ss
23,Format::riffDateTime1,MMM dd hh:mm:ss yyyy
24,Format::riffDateTime2,yyyy-MM-dd hh:mm:ss
*/

// Test case for a one-year folder
TEST(fvaParseDirNameTest, OneYearFolder)
{
    QString dirName = "2022";
    QDateTime from, to;
    FvaFmtContext ctx; // Provide a valid FvaFmtContext object
    ctx.fvaDirNameYear = "yyyy"; // Set the format for year parsing

    FVA_EXIT_CODE result = fvaParseDirName(dirName, from, to, ctx);

    EXPECT_EQ(result, FVA_NO_ERROR);
    // Add additional assertions for the values of 'from' and 'to'
}

// Test case for a year period
TEST(fvaParseDirNameTest, YearPeriod)
{
    QString dirName = "2022-2023";
    QDateTime from, to;
    FvaFmtContext ctx; // Provide a valid FvaFmtContext object
    ctx.fvaDirNameYear = "yyyy"; // Set the format for year parsing

    FVA_EXIT_CODE result = fvaParseDirName(dirName, from, to, ctx);

    EXPECT_EQ(result, FVA_NO_ERROR);
    // Add additional assertions for the values of 'from' and 'to'
}

// Test case for a one-day event
TEST(fvaParseDirNameTest, OneDayEvent)
{
    QString dirName = "2022-01-01";
    QDateTime from, to;
    FvaFmtContext ctx; // Provide a valid FvaFmtContext object
    ctx.fvaDirName = "yyyy-MM-dd"; // Set the format for date parsing

    FVA_EXIT_CODE result = fvaParseDirName(dirName, from, to, ctx);

    EXPECT_EQ(result, FVA_NO_ERROR);
    // Add additional assertions for the values of 'from' and 'to'
}

// Test case for one day and several events
TEST(fvaParseDirNameTest, OneDaySeveralEvents)
{
    QString dirName = "2022-01-01#3";
    QDateTime from, to;
    FvaFmtContext ctx; // Provide a valid FvaFmtContext object
    ctx.fvaDirName = "yyyy-MM-dd"; // Set the format for date parsing

    FVA_EXIT_CODE result = fvaParseDirName(dirName, from, to, ctx);

    EXPECT_EQ(result, FVA_NO_ERROR);
    // Add additional assertions for the values of 'from' and 'to'
}

// Test case for a period
TEST(fvaParseDirNameTest, Period)
{
    QString dirName = "2022-01-01-03";
    QDateTime from, to;
    FvaFmtContext ctx; // Provide a valid FvaFmtContext object
    ctx.fvaDirName = "yyyy-MM-dd"; // Set the format for date parsing

    FVA_EXIT_CODE result = fvaParseDirName(dirName, from, to, ctx);

    EXPECT_EQ(result, FVA_NO_ERROR);
    // Add additional assertions for the values of 'from' and 'to'
}

// Test case for months-day period
TEST(fvaParseDirNameTest, MonthsDayPeriod)
{
    QString dirName = "2022-01-01-01.31";
    QDateTime from, to;
    FvaFmtContext ctx; // Provide a valid FvaFmtContext object
    ctx.fvaDirName = "yyyy-MM-dd"; // Set the format for date parsing

    FVA_EXIT_CODE result = fvaParseDirName(dirName, from, to, ctx);

    EXPECT_EQ(result, FVA_NO_ERROR);
    // Add additional assertions for the values of 'from' and 'to'
}

// Test case for an invalid folder name
TEST(fvaParseDirNameTest, InvalidFolderName)
{
    QString dirName = "invalid";
    QDateTime from, to;
    FvaFmtContext ctx; // Provide a valid FvaFmtContext object
    ctx.fvaDirName = "yyyy-MM-dd"; // Set the format for date parsing

    FVA_EXIT_CODE result = fvaParseDirName(dirName, from, to, ctx);

    EXPECT_EQ(result, FVA_ERROR_WRONG_FOLDER_NAME);
}