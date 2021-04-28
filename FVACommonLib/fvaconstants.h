#ifndef FVACONSTS_H
#define FVACONSTS_H

#include <QtCore/QString>
#include <QtCore/QMap>

/*!
 * \brief it enumerates internal valuable file system types
 */
enum FVA_FS_TYPE
{
	FVA_FS_TYPE_UNKNOWN = 0,
	FVA_FS_TYPE_IMG		= 1,
	FVA_FS_TYPE_VIDEO	= 2,
	FVA_FS_TYPE_AUDIO	= 3,

	FVA_FS_TYPE_DIR		= 4,
};

/*!
* \brief it enumerates internal valuable device types
*/
enum FVA_DEVICE_TYPE
{
	DEVICE_TYPE_PHOTO_UKNOWN	= 0,
	DEVICE_TYPE_PHOTO_VIDEO		= 1,
	DEVICE_TYPE_PHOTO_SCANER	= 1,
};

/*!
 * \brief internal file names
 */
const QString	FVA_BACKGROUND_MUSIC_FILE_NAME		= "bgmusic.mid";

/*!
 * \brief id for any object that is not defined
 */
const int FVA_UNDEFINED_ID	= -1;

/*!
* \brief name of SQLlite db to work with
*/
const QString	FVA_DB_NAME = "digikam4.db";

/*!
* \brief language to be used TODO to remove
*/
#define FVA_LANGUAGE_ENG
// #define FVA_LANGUAGE_RUS

#endif // FVACONSTS_H
