/*!
* \file FVADeviceContext.h
* \copyright Copyright 2021 FVA Software. All rights reserved. This file is released under the XXX License.
* \author Dima Nikulin.
* \version 0.29
* \date  2014-2021
*/
#ifndef DEVICE_CONTEXT_H_
#define DEVICE_CONTEXT_H_

#include <QtCore/QString>

#include "fvadevice.h"

/*!
 * \brief to keep devices information to control the flow
 */
class DeviceContext
{
	public:

		/*!
		 * \brief human - readable device name
		 */
		QString matchedDeviceName;

		/*!
		 * \brief full the devices map
		 */
		DEVICE_MAP fullDeviceMap;

		/*!
		* \brief matched the devices map
		*/
		DEVICE_MAP deviceMap;
};
#endif // DEVICE_CONTEXT_H_