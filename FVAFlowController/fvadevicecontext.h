#ifndef DEVICE_CONTEXT_H_
#define DEVICE_CONTEXT_H_

#include <QtCore/QString>

#include "data/fvadevice.h"

/*!
 * \brief to keep devices information for the flow
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