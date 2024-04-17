// Test case for verifying the initialization of fvaDevice object
TEST(fvaDeviceTests, InitializationTest)
{
    // Arrange
    fvaDevice device;

    // Act

    // Assert
    EXPECT_EQ(device.deviceId, 0);
    EXPECT_EQ(device.ownerName, "");
    EXPECT_EQ(device.ownerId, 0);
    EXPECT_EQ(device.linkedName, "");
    EXPECT_EQ(device.guiName, "");
    EXPECT_EQ(device.type, FVA_DEVICE_TYPE::UNKNOWN);
}

// Test case for setting and getting the device identifier
TEST(fvaDeviceTests, DeviceIdTest)
{
    // Arrange
    fvaDevice device;
    int deviceId = 123;

    // Act
    device.deviceId = deviceId;

    // Assert
    EXPECT_EQ(device.deviceId, deviceId);
}

// Test case for setting and getting the owner name
TEST(fvaDeviceTests, OwnerNameTest)
{
    // Arrange
    fvaDevice device;
    QString ownerName = "John Doe";

    // Act
    device.ownerName = ownerName;

    // Assert
    EXPECT_EQ(device.ownerName, ownerName);
}

// Test case for setting and getting the owner identifier
TEST(fvaDeviceTests, OwnerIdTest)
{
    // Arrange
    fvaDevice device;
    int ownerId = 456;

    // Act
    device.ownerId = ownerId;

    // Assert
    EXPECT_EQ(device.ownerId, ownerId);
}

// Test case for setting and getting the linked name
TEST(fvaDeviceTests, LinkedNameTest)
{
    // Arrange
    fvaDevice device;
    QString linkedName = "Brand Model";

    // Act
    device.linkedName = linkedName;

    // Assert
    EXPECT_EQ(device.linkedName, linkedName);
}

// Test case for setting and getting the GUI name
TEST(fvaDeviceTests, GuiNameTest)
{
    // Arrange
    fvaDevice device;
    QString guiName = "Device Name";

    // Act
    device.guiName = guiName;

    // Assert
    EXPECT_EQ(device.guiName, guiName);
}

// Test case for setting and getting the device type
TEST(fvaDeviceTests, DeviceTypeTest)
{
    // Arrange
    fvaDevice device;
    FVA_DEVICE_TYPE type = FVA_DEVICE_TYPE::CAMERA;

    // Act
    device.type = type;

    // Assert
    EXPECT_EQ(device.type, type);
}