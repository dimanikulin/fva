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
 * \brief internal file names
 */
const QString	FVA_DESCRIPTION_FILE_NAME			= "description.csv";
const QString	FVA_DIR_DESCRIPTION_FILE_NAME		= "folderDescription.json";
const QString	FVA_BACKGROUND_MUSIC_FILE_NAME		= "bgmusic.mid";

const QString	FVA_TARGET_FOLDER_NAME				= "TARGET_FOLDER_NAME"; 

/*!
 * \brief id for any object that is not defined
 */
const int FVA_UNDEFINED_ID	= -1;

class fvaDevice;
typedef QMap<int, fvaDevice> DEVICE_MAP;

typedef QMap< int, QStringList > DESCRIPTIONS_MAP;

class fvaFile;
typedef QMap<QString, fvaFile> FVA_FILE_INFO_MAP;

#endif // FVACONSTS_H
