#ifndef FVACOMMONLIB_H
#define FVACOMMONLIB_H

#include <QString>
#include <QStringList>
#include <QVector>
#include <QDateTime>
#include <QVariantMap>
#include <QTGui/QLabel>
#include <QTGui/QTreeWidget>

typedef QMap< QString, QStringList > DESCRIPTIONS_MAP;

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
	FVA_ERROR_CANT_CREATE_DIR					= 1032,
	FVA_ERROR_CANT_CREATE_FVA_SQL				= 1033,
	FVA_ERROR_CANT_RENAME_FILE_DESC				= 1034,
	FVA_ERROR_CANT_FIND_FVA_DB					= 1035,
	FVA_ERROR_CANT_OPEN_FVA_DB					= 1036,
	FVA_ERROR_CANT_LOAD_FVA_DB					= 1037,
	FVA_ERROR_NOT_IMPLEMENTED					= 1038,
	FVA_ERROR_CANT_OPEN_ID_FILE					= 1039,
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

/*!
 * \brief it loads FVA dictionaries from file 
 * \param file to load dictionaries from 
 * \param output output parameter to be filled up by json structures
 * \param error - human-readable description of error if any 
 * \returns it returns code of error if any or FVA_NO_ERROR if loading was successful
 */
FVA_ERROR_CODE fvaLoadDictionary( const QString& file, QVariantMap& output, QString& error );

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

/*!
 * \brief it created filter tree in gui widgets with two levels
 * \param pMainWnd - window keeping tree widget  
 * \param pTreeWidget - widget to build tree in
 * \param rootLevel - root level dictionary
 * \param level - second level dictionary
 */
void fvaBuildFilterTree(QWidget* pMainWnd, 
					QTreeWidget* pTreeWidget, 
					const QVariantList& rootLevel, 
					const QVariantList& level,
					QIcon* rootIcon,
					QIcon* icon);

/*!
 * \brief it created filter tree in gui widgets
  * \param pMainWnd - window keeping tree widget  
 * \param pTreeWidget - widget to build tree in
 * \param devices - need to insert devices
 */
void fvaBuildPeopleFilterTree(QWidget* pMainWnd, QTreeWidget* pTreeWidget, bool devices, const QVariantMap& dict);

/*!
 * \brief it finds all checked item ID in gui widget tree
 * \param item - to search in
 * \param Ids - arrau to fill by id of checked items
 * \param devices - need to insert devices
 */
void fvaFindCheckedItem(QTreeWidgetItem *item, QVector<unsigned int>& Ids);


/*!
 * \brief it recursivle update checked 
 * \param item - to update in
 * \param column - column
 */
void fvaUpdateChecks(QTreeWidgetItem *item, int column);

/*!
 * \brief it convert identifiers list from string to int vector
 */
QVector<unsigned int> fvaStringToIds(const QString& strList);

/*!
 * \brief it loads fva items from DB as a tree 
 * \param rootPath root folder of FVA structure
 * \param rootItem root fva item to start loading from
 * \param DBPath path to fva DB
 * \param error error description to be filled up
 * \param number count of items to be proceeded
 * \returns it returns code of error if any or FVA_NO_ERROR if loading was successful
 */
FVA_ERROR_CODE fvaLoadItems(const QString& rootPath, fvaItem* rootItem, const QString& DBPath, QString& error, int& number);

/*!
 * \brief it recursivly filters FVA tree
 * \param filter - filter condition set
 * \param fvaitem - fva tree item
 * \param defFilterDataTime - data+time that is defined as default one
 */
void fvaFilterTree( const fvaFilter& filter, fvaItem* fvaitem, const QDateTime& defFilterDataTime );

#define FILL_COMB_FROM_DICT(dict,combo) \
	vlist = m_dictionaries[dict].toList();\
	combo->clear(); \
	for ( auto i = vlist.begin(); i != vlist.end() ; ++i )\
		combo->addItem ( i->toMap()["name"].toString(), i->toMap()["ID"].toString() );

/*!
 * \brief it returns a value by a name from resuls and deletes the name and the value after
 * \param fieldName - name of field
 * \param result - map to search in and to remove from
 * \return it returns a value
 */
QString fvaDVget(const QString& fieldName, QVariantMap& result );

/*!
 * \brief it returns an ID value from file by file path provided
 * \param file - path to file that contains the ID
 * \param int& - variable to fill up by ID loaded from the file
 * \returns it returns code of error if any or FVA_NO_ERROR if loading was successful
 */
FVA_ERROR_CODE fvaGetIDFromFile(const QString& file, int& ID);

/*!
 * \brief it saves an ID value in file by file path provided (it will delete previous content!)
 * \param file - path to file that will contain the ID
 * \param int - ID to save
 * \returns it returns code of error if any or FVA_NO_ERROR if saving was successful
 */
FVA_ERROR_CODE fvaSaveIDInFile(const QString& file, int ID);

#endif // FVACOMMONLIB_H

