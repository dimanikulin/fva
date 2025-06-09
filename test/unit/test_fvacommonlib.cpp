#include <gtest/gtest.h>
#include "fvacommonlib.h"


// Test case for a one-year folder
TEST(fvaParseDirNameTest, OneYearFolder)
{
    QString dirName = "2022";
    QDateTime from, to;
    FvaFmtContext ctx; // Provide a valid FvaFmtContext object

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

    FVA_EXIT_CODE result = fvaParseDirName(dirName, from, to, ctx);

    EXPECT_EQ(result, FVA_ERROR_WRONG_FOLDER_NAME);
}