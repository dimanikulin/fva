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
};

typedef QMap<int, fvaDevice> DEVICE_MAP;

#endif // FVADEVICE_H