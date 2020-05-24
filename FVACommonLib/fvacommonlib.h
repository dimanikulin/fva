#ifndef FVACOMMONLIB_H
#define FVACOMMONLIB_H

#include <QString>
#include <QVariantMap>


////////////////////////////////TODOTODOTODOTODOTODOTODOTODOTODOTODO/////////////////////////////////////////////
// 1. make warning level as 4 for all projects
// 2. threat warning as errors for all projects
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/*!
 * \brief it enumerates internal valuable file types
 */
enum FVA_FILE_TYPE
{
	FVA_FILE_TYPE_UNKNOWN,
	FVA_FILE_TYPE_IMG,
	FVA_FILE_TYPE_VIDEO,
	FVA_FILE_TYPE_AUDIO
};

/*!
 * code errors enumeration
 */
enum FVA_ERROR_CODE
{
	FVA_NO_ERROR									= 0,
	
	FVA_ERROR_NOT_ENOUGH_ARG						= 1001,
	FVA_ERROR_INPUT_DIR_NOT_EXIST_ARG				= 1002,
	FVA_ERROR_CANT_OPEN_DIR_DESC					= 1003,
	FVA_ERROR_CANT_RENAME_DIR_DESC					= 1004,
	FVA_ERROR_CANT_OPEN_NEW_DIR_DESC				= 1005,
	FVA_ERROR_NOT_SUPPORTED_RO_MODE					= 1006,
	FVA_ERROR_CANT_RENAME_FILE						= 1007,
	FVA_ERROR_SEQUENCE								= 1008,
	FVA_ERROR_CANT_OPEN_INPUT_FILE					= 1009,
	FVA_ERROR_UKNOWN_CMD							= 1010,
	FVA_ERROR_CANT_FIND_MANDATORY_FIELDS			= 1011,
	FVA_ERROR_INCORRECT_FORMAT						= 1012,
	FVA_ERROR_CANT_REMOVE_FILE_OR_DIR				= 1013,
	FVA_ERROR_CANT_OPEN_FILE_DESC					= 1014,
	FVA_ERROR_CANT_FIND_DIR_DESC					= 1015,
};

/*!
 * \brief internal file names
 */
const QString	DESCRIPTION_FILE_NAME				= "description.csv";
const QString	OLD_DESCRIPTION_FOLDER_NAME			= "folderDescription.json_old";
const QString	DESCRIPTION_FOLDER_NAME				= "folderDescription.json";
const QString	BACKGROUND_MUSIC_FILE_NAME			= "bgmusic.mid";

/*!
 * \brief how many supported files should be in a folder
 */
const unsigned int DEFAULT_MIN_COUNT_FILES_IN_DIR	= 3;

/*!
 * \brief it loads folder description 
 * \param outputJson output parameter to be filled up by json structure
 * \param error - human-readable description of error if any 
 * \returns it returns code of error if any or FVA_NO_ERROR if loading was successful
 */
FVA_ERROR_CODE getFolderDescription( const QString& folder, QVariantMap& outputJson, QString& error );

#endif // FVACOMMONLIB_H
