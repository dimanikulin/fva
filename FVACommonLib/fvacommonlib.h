#ifndef FVACOMMONLIB_H
#define FVACOMMONLIB_H

////////////////////////////////TODOTODOTODOTODOTODOTODOTODOTODOTODO/////////////////////////////////////////////
// 1. make warning level as 4 for all projects
// 2. threat warning as errors for all projects
// 3. https://www.projectoxford.ai/demo/face#detection
// 4. change solutin/projects name to AVF 
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <QString>
#include <QStringList>
#include <QVector>
#include <QDateTime>
#include <QVariantMap>
#include <QTGui/QLabel>

typedef QMap< QString, QStringList > DESCRIPTIONS_MAP;

#define _NEW_DICTIONARY_
#include "fvacommondata.h"

/*!
 * code errors enumeration
 */
enum FVA_ERROR_CODE
{
	FVA_NO_ERROR								= 0,
	
	FVA_ERROR_NOT_ENOUGH_ARG					= 1001,
	FVA_ERROR_INPUT_DIR_NOT_EXIST_ARG			= 1002,
	FVA_ERROR_CANT_OPEN_DIR_DESC				= 1003,
	FVA_ERROR_CANT_RENAME_DIR_DESC				= 1004,
	FVA_ERROR_CANT_OPEN_NEW_DIR_DESC			= 1005,
	FVA_ERROR_NOT_SUPPORTED_RO_MODE				= 1006,
	FVA_ERROR_CANT_RENAME_FILE					= 1007,
	FVA_ERROR_SEQUENCE							= 1008,
	FVA_ERROR_CANT_OPEN_INPUT_FILE				= 1009,
	FVA_ERROR_UKNOWN_CMD						= 1010,
	FVA_ERROR_CANT_FIND_MANDATORY_FIELDS		= 1011,
	FVA_ERROR_INCORRECT_FORMAT					= 1012,
	FVA_ERROR_CANT_REMOVE_FILE_OR_DIR			= 1013,
	FVA_ERROR_CANT_OPEN_FILE_DESC				= 1014,
	FVA_ERROR_CANT_FIND_DIR_DESC				= 1015,
	FVA_ERROR_EMPTY_DEV_ID						= 1016,
	FVA_ERROR_CANT_FIND_DICTIONARIES			= 1017,
	FVA_ERROR_CANT_OPEN_DICTIONARIES			= 1018,
	FVA_ERROR_CANT_LOAD_DICTIONARIES			= 1019,
	FVA_ERROR_WRONG_PARAMETERS					= 1020,
	FVA_ERROR_WRONG_FOLDER_NAME					= 1021,
	FVA_ERROR_WRONG_FILE_NAME					= 1022,
	FVA_ERROR_CANT_SAVE_DICTIONARIES			= 1023,
	FVA_ERROR_NOT_SUPPORTED_FILE				= 1024,
	FVA_ERROR_VIDEO_FIRST						= 1025,
	FVA_ERROR_PANORAM_FILE						= 1026,
	FVA_ERROR_EMPTY_DEVICE_NAME					= 1027,
	FVA_ERROR_NON_UNIQUE_DEVICE_NAME			= 1028,
	FVA_ERROR_UKNOWN_DEVICE						= 1029,
	FVA_ERROR_TOO_LITTLE_FILES					= 1030,
	FVA_ERROR_CANT_MOVE_DIR						= 1031,
};

/*!
 * \brief internal file names
 */
const QString	FVA_DESCRIPTION_FILE_NAME			= "description.csv";
const QString	FVA_DIR_DESCRIPTION_FILE_NAME		= "folderDescription.json";
const QString	FVA_BACKGROUND_MUSIC_FILE_NAME		= "bgmusic.mid";

#ifdef _NEW_DICTIONARY_
const QString	FVA_DICTIONARY_NAME					= "dct.db";
#else
const QString	FVA_DICTIONARY_NAME					= "data.json";
#endif
/*!
 * \brief how many supported files should be in a folder
 */
const unsigned int FVA_DEFAULT_MIN_COUNT_FILES_IN_DIR	= 3;

/*!
 * \brief it loads folder description 
 * \brief folder to load description from
 * \param outputJson output parameter to be filled up by json structure
 * \param error - human-readable description of error if any 
 * \returns it returns code of error if any or FVA_NO_ERROR if loading was successful
 */
FVA_ERROR_CODE fvaGetFolderDescription( const QString& folder, QVariantMap& outputJson, QString& error );

/*!
 * \brief it creates folder description file with content
 * \param path - file path to create folder decsription
 * \param content - json data to be written
 * \param error - human-readable description of error if any to be filled up
 * \returns it returns code of error if any or FVA_NO_ERROR if writing was successful
 */
FVA_ERROR_CODE fvaCreateFolderDescription (const QString& path, const QString& content, QString& error);

/*!
 * \brief it loads FVA dictionaries from file 
 * \param file to load dictionaries from 
 * \param outputJson output parameter to be filled up by json structures
 * \param error - human-readable description of error if any 
 * \returns it returns code of error if any or FVA_NO_ERROR if loading was successful
 */
FVA_ERROR_CODE fvaLoadDictionary( const QString& file, QVariantMap& outputJson, QString& error );

/*!
 * \brief it saves FVA dictionaries to file 
 * \param file to save dictionaries to 
 * \param outputJson input parameter to be saved
 * \param error - human-readable description of error if any 
 * \returns it returns code of error if any or FVA_NO_ERROR if saving was successful
 */
FVA_ERROR_CODE fvaSaveDictionary( const QString& file, QVariantMap& inputJson, QString& error );

/*!
* \brief it converts file extention to file type if it is possible
*/
FVA_FS_TYPE fvaConvertFileExt2FileType ( const QString& type );

/*!
* \brief it answers if file is internal purpose for
*/
bool fvaIsInternalFile( const QString& fileName );

/*!
* \brief it answers if file is supported type of  (by its file extention)
*/
bool fvaIsFVAFile( const QString& extention );

/*!
 * \brief it makes showing images in the most common way
 * \param fileName - path to file
 * \param imgLabel to show image at
 * \param text - text to draw over an image
 * \returns it returns code of error if any or FVA_NO_ERROR if showing was successful
 */
FVA_ERROR_CODE fvaShowImage( const QString& fileName, QLabel* imgLabel, const QString& text);

/*!
 * \brief it tries to parse dir name into period of date
 * \param dirName directory name to be parsed
 * \param from to fill as a date from
 * \param to to fill as a date to
 * \returns it returns code of error if any or FVA_NO_ERROR if parsing was successful
 */
FVA_ERROR_CODE fvaParseDirName( const QString& dirName, QDateTime& from, QDateTime& to );

/*!
 * \brief it tries to parse file name into date
 * \param fileName file name to be parsed
 * \param date a date to be filled up
 * \returns it returns code of error if any or FVA_NO_ERROR if parsing was successful
 */
FVA_ERROR_CODE fvaParseFileName( const QString& fileName, QDateTime& date );

/*!
 * \brief it tries to load device id map from dictionary
 * \param deviceIds a map to be filled up
 * \param dictPath path to dictionary to load map from
 * \returns it returns code of error if any or FVA_NO_ERROR if loading was successful
 */
FVA_ERROR_CODE fvaLoadDeviceMapFromDictionary(DEVICE_MAP& deviceMap, const QString& dictPath); 

/*!
 * \brief it returns device id from map loaded
 * \param deviceIds a map to be find in
 * \param pathToFile path to file to get device name from
 * \param deviceName contains device link name
 * \returns it returns map of devices found or empty map if not
 */
DEVICE_MAP fvaGetDeviceMapForImg(const DEVICE_MAP& deviceMap, const QString& pathToFile, QString& deviceName); 

/*!
 * \brief it tries to get time a video taken at from header
 * \param pathToFile a path to video file
 * \param error error description
 * \returns it returns time taken
 */
QDateTime fvaGetVideoTakenTime(const QString& pathToFile, QString& error);

#define FILL_COMB_FROM_DICT(dict,combo) \
	vlist = m_dictionaries[dict].toList();\
	combo->clear(); \
	for ( auto i = vlist.begin(); i != vlist.end() ; ++i )\
		combo->addItem ( i->toMap()["name"].toString(), i->toMap()["ID"].toString() );

#endif // FVACOMMONLIB_H
