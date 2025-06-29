#include <gtest/gtest.h>
#include "fvaconfiguration.h"

/*
// Test case for getting an existing parameter value
TEST(FvaConfigurationTest, GetExistingParamValue)
{
    FvaConfiguration config;
    QString paramName = "ID";
    uint expectedValue = 12345;
    
    // Set up the configuration with the parameter
    config.setParam(paramName, expectedValue);
    config.setParam("Name", "Parameter1");
    config.setParam("Value", true);

    uint actualValue;
    FVA_EXIT_CODE result = config.getParamAsUint(paramName, actualValue);

    EXPECT_EQ(result, FVA_NO_ERROR);
    EXPECT_EQ(actualValue, expectedValue);
}

// Test case for getting a non-existing parameter value
TEST(FvaConfigurationTest, GetNonExistingParamValue)
{
    FvaConfiguration config;
    QString paramName = "InvalidParam";
    QString actualValue;

    // Set up the configuration with some parameters
    config.setParam("Name", "Parameter1");
    config.setParam("Value", true);


    FVA_EXIT_CODE result = config.getParamAsString(paramName, actualValue);

    EXPECT_EQ(result, FVA_ERROR_CANT_GET_PARAM);
    EXPECT_TRUE(actualValue.isEmpty());
}

// Test case for getting a parameter value with empty configuration
TEST(FvaConfigurationTest, GetParamValueWithEmptyConfig)
{
    FvaConfiguration config;
    QString paramName = "ID";
    QString actualValue;

    FVA_EXIT_CODE result = config.getParamAsString(paramName, actualValue);

    EXPECT_EQ(result, FVA_ERROR_CANT_GET_PARAM);
    EXPECT_TRUE(actualValue.isEmpty());
}
// Test case for loading a valid configuration file
TEST(FvaConfigurationTest, LoadValidConfigFile)
{
    FvaConfiguration config;
    QString filePath = "/path/to/valid/config.csv";

    FVA_EXIT_CODE result = config.load(filePath);

    EXPECT_EQ(result, FVA_NO_ERROR);
    // Add additional assertions to verify the loaded configuration
}

// Test case for loading an invalid configuration file
TEST(FvaConfigurationTest, LoadInvalidConfigFile)
{
    FvaConfiguration config;
    QString filePath = "/path/to/invalid/config.csv";

    FVA_EXIT_CODE result = config.load(filePath);

    EXPECT_NE(result, FVA_NO_ERROR);
}

// Test case for saving the configuration to a file
TEST(FvaConfigurationTest, SaveConfigToFile)
{
    FvaConfiguration config;
    QString filePath = "/path/to/save/config.csv";

    // Set up the configuration with some data
    config.setParam("Name", "Parameter1");
    config.setParam("Value", true);
    config.setParam("ID", "12345");

    FVA_EXIT_CODE result = config.save(filePath);

    EXPECT_EQ(result, FVA_NO_ERROR);
    // Add additional assertions to verify the saved configuration file
    QString savedValue;
    result = config.getParamAsString("Name", savedValue);
    EXPECT_EQ(result, FVA_NO_ERROR);
    EXPECT_EQ(savedValue, "Parameter1");
    result = config.getParamAsString("Value", savedValue);
    EXPECT_EQ(result, FVA_NO_ERROR);
    EXPECT_EQ(savedValue, true);
}

// Test case for getting a parameter value as string
TEST(FvaConfigurationTest, GetParamAsString)
{
    FvaConfiguration config;
    QString paramName = "ID";
    QString expectedValue = "12345";
    QString actualValue;

    // Set up the configuration with the parameter
    config.setParam(paramName, expectedValue);
    config.setParam("Name", "Parameter1");
    config.setParam("Value", true);

    FVA_EXIT_CODE result = config.getParamAsString(paramName, actualValue);

    EXPECT_EQ(result, FVA_NO_ERROR);
    EXPECT_EQ(actualValue, expectedValue);
}

// Test case for getting a parameter value as boolean
TEST(FvaConfigurationTest, GetParamAsBoolean)
{
    FvaConfiguration config;
    QString paramName = "Flag";
    bool expectedValue = true;
    bool actualValue;

    // Set up the configuration with the parameter
    config.setParam(paramName, expectedValue);
    config.setParam("Name", "Parameter1");
    uint paramValue = 10;
    config.setParam("Count", paramValue);

    FVA_EXIT_CODE result = config.getParamAsBoolean(paramName, actualValue);

    EXPECT_EQ(result, FVA_NO_ERROR);
    EXPECT_EQ(actualValue, expectedValue);
}

// Test case for getting a parameter value as unsigned integer
TEST(FvaConfigurationTest, GetParamAsUint)
{
    FvaConfiguration config;
    QString paramName = "Count";
    uint expectedValue = 10;
    uint actualValue;

    // Set up the configuration with the parameter
    config.setParam(paramName, expectedValue);
    config.setParam("Name", "Parameter1");
    config.setParam("Flag", true);

    FVA_EXIT_CODE result = config.getParamAsUint(paramName, actualValue);

    EXPECT_EQ(result, FVA_NO_ERROR);
    EXPECT_EQ(actualValue, expectedValue);
}

// Test case for setting a parameter value as unsigned integer
TEST(FvaConfigurationTest, SetParamUint)
{
    FvaConfiguration config;
    QString paramName = "Count";
    uint paramValue = 10;

    FVA_EXIT_CODE result = config.setParam(paramName, paramValue);

    EXPECT_EQ(result, FVA_NO_ERROR);

    // Add additional assertions to verify the updated configuration
    uint actualValue;
    result = config.getParamAsUint(paramName, actualValue);
    EXPECT_EQ(result, FVA_NO_ERROR);
    EXPECT_EQ(actualValue, paramValue);
}

// Test case for setting a parameter value as boolean
TEST(FvaConfigurationTest, SetParamBoolean)
{
    FvaConfiguration config;
    QString paramName = "Flag";
    bool paramValue = true;

    FVA_EXIT_CODE result = config.setParam(paramName, paramValue);

    EXPECT_EQ(result, FVA_NO_ERROR);

    // Add additional assertions to verify the updated configuration
    bool actualValue;
    result = config.getParamAsBoolean(paramName, actualValue);
    EXPECT_EQ(result, FVA_NO_ERROR);
    EXPECT_EQ(actualValue, paramValue);
}

// Test case for setting a parameter value as string
TEST(FvaConfigurationTest, SetParamString)
{
    FvaConfiguration config;
    QString paramName = "Name";
    QString paramValue = "John Doe";

    FVA_EXIT_CODE result = config.setParam(paramName, paramValue);

    EXPECT_EQ(result, FVA_NO_ERROR);
    
    // Add additional assertions to verify the updated configuration
    QString actualValue;
    result = config.getParamAsString(paramName, actualValue);
    EXPECT_EQ(result, FVA_NO_ERROR);
    EXPECT_EQ(actualValue, paramValue);
}*/