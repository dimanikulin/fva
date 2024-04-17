/*!
 * \file fvadevice.h 
 * \copyright Copyright 2021 FVA Software. All rights reserved. This file is released under the XXX License.
 * \author Dima Nikulin.
 * \version 0.29
 * \date  2014-2021
 */
#ifndef FVADEVICE_H
#define FVADEVICE_H

#include <QtCore/QString>
#include <QtCore/QMap>

#include "fvaconstants.h"

/*!
 * \brief it describes whole information about device
 */
class fvaDevice
{
	public:
		/*
		 * device identifier from device dictionary
		 */
		int deviceId;

		/*
		 * owner name from people dictionary
		 */
		QString ownerName;

		/*
		 * owner identifier from people dictionary
		 */
		int ownerId;

		/*
		 * device name to register in dictionary(usually <brand name> + <device model>)
		 */
		QString linkedName;

		/*
		 * device name to show in GUI
		 */
		QString guiName;

		/*
		 * device type
		 */
		FVA_DEVICE_TYPE type;

		/*
		 * cons
		 */
		fvaDevice(){}
		
    EXPECT_EQ(device.deviceId, 0);
    EXPECT_EQ(device.ownerName, "");
    EXPECT_EQ(device.ownerId, 0);
    EXPECT_EQ(device.linkedName, "");
    EXPECT_EQ(device.guiName, "");
    EXPECT_EQ(device.type, FVA_DEVICE_TYPE::DEVICE_TYPE_PHOTO_UKNOWN);

};

typedef QMap<int, fvaDevice> DEVICE_MAP;

#endif // FVADEVICE_H