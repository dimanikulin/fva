#ifndef FVACOMMONLIB_H
#define FVACOMMONLIB_H

////////////////////////////////TODOTODOTODOTODOTODOTODOTODOTODOTODO/////////////////////////////////////////////
// 1. make warning level as 4 for all projects
// 2. threat warning as errors for all projects
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <QString>
#include <QStringList>
#include <QVector>
#include <QDateTime>
#include <QVariantMap>
#include <QTGui/QLabel>

typedef QMap< QString, QStringList > DESCRIPTIONS_MAP;

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
};

/*!
 * \brief internal file names
 */
const QString	FVA_DESCRIPTION_FILE_NAME			= "description.csv";
const QString	FVA_OLD_DESCRIPTION_FILE_NAME		= "description_old.csv";
const QString	FVA_OLD_DIR_DESCRIPTION_FILE_NAME	= "folderDescription.json_old";
const QString	FVA_DIR_DESCRIPTION_FILE_NAME		= "folderDescription.json";
const QString	FVA_BACKGROUND_MUSIC_FILE_NAME		= "bgmusic.mid";
const QString	FVA_DICTIONARY_NAME					= "data.json";

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
* \brief it converts file extention to file type if it is possible
*/
FVA_FILE_TYPE fvaConvertFileExt2FileType ( const QString& type );

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
 * \returns it returns code of error if any or FVA_NO_ERROR if showing was successful
 */
FVA_ERROR_CODE fvaShowImage( const QString& fileName, QLabel* imgLabel );

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
 * \brief it keeps whole information about a foto-video-audio element
 */
class fvaItem
{
	public : 

		fvaItem ()
		{
			isFolder			= false;
			isFiltered			= true;
			hasDescriptionData	= false;
			deviceId			= 0;
			personId			= 0;
			scanerId			= 0;
		}
		virtual ~fvaItem ()
		{
			for (auto idChild = children.begin(); idChild != children.end(); ++idChild)
			{
				if (idChild == nullptr)
					continue;
				delete *idChild;
				*idChild = nullptr;
			}
		}

		/*!
		 * is it folder or file
		 */
		bool					isFolder;

		/*!
		 * for folder it is event date period, for file it is date taken(dateFrom only)
		 * both are filesystem names
		 */
		QDateTime				dateFrom;
		QDateTime				dateTo;

		/*!
		 * does this item have any description data
		 */
		bool					hasDescriptionData;

		/*!
		 * set of place id 
		 */
		QVector<unsigned int>	placeIds;

		/*!
		 * people list ids 
		 */
		QVector<unsigned int>	peopleIds;

		/*!
		 * id of foto device 
		 */
		unsigned int			deviceId;

		/*!
		 * id of person who took this foto/video/audio item
		 */
		unsigned int			personId;

		/*!
		 * id of scaner
		 */
		unsigned int			scanerId;

		/*!
		 * folder event or file decsription
		 */
		QString					eventOrDesc;

		/*!
		 * folder tags or file comment
		 */
		QString					tagsOrComment;

		/*!
		 * children
		 */
		QVector<fvaItem*>		children;

		/*!
		 * absolute path to item
		 */
		QString					fsFullPath;

		/*!
		 * does this item fit filtration conditions
		 */
		bool					isFiltered;

		/*!
		 * folder to show together with this folder
		 */
		QString					linkedFolder;
		
		/*! 
		 * TODO delete this field
		 */
		QString					fsName;

		/*!
		 * structure of description file 
		 */
		QStringList				descTitles; 

		/*!
		 * content of description file 
		 */
		DESCRIPTIONS_MAP		decsItems;

};
/*!
 * it keeps data to perform filtration
 */
class fvaFilter
{
	public :
	
		/*!
		 * date to filter from
		 */
		QDateTime				dateFrom;

		/*!
		 * date to filter to
		 */
		QDateTime				dateTo;

		/*!
		 * places to be filtered
		 */
		QVector<unsigned int>	placeIds;

		/*!
		 * people to be entered
		 */
		QVector<unsigned int>	peopleIds;

		/*!
		 * list of id of foto devices
		 */
		QVector<unsigned int>	deviceIds;

		/*!
		 * folder event or file decsription
		 */
		QString					eventOrDesc;

};

#define FILL_COMB_FROM_DICT(dict,combo) \
	vlist = dictionaries[dict].toList();\
	for ( auto i = vlist.begin(); i != vlist.end() ; ++i )\
		combo->addItem ( i->toMap()["name"].toString(), i->toMap()["ID"].toString() );

#endif // FVACOMMONLIB_H
