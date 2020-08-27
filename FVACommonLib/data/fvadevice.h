#ifndef FVAPEOPLE_H
#define FVAPEOPLE_H

#include <QtCore/QString>

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

#endif // FVAPEOPLE_H