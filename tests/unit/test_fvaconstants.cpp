#include <gtest/gtest.h>
#include "fvaconstants.h"

// Test case to verify the value of FVA_BACKGROUND_MUSIC_FILE_NAME
TEST(FVAConstantsTests, BackgroundMusicFileNameTest)
{
    // Assert
    EXPECT_EQ(FVA_BACKGROUND_MUSIC_FILE_NAME, "bgmusic.mid");
}