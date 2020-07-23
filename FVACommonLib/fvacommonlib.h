#ifndef FVACOMMONLIB_H
#define FVACOMMONLIB_H

#include <QString>
#include <QStringList>
#include <QVector>
#include <QDateTime>
#include <QVariantMap>
#include <QTGui/QLabel>
#include <QTGui/QTreeWidget>

#include "fvaconstants.h"
#include "fvaerrors.h"


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
 * \brief it returns device id from map loaded
 * \param deviceIds a map to be find in
 * \param pathToFile path to file to get device name from
 * \param deviceName contains device link name
 * \returns it returns map of devices found or empty map if not
 */
DEVICE_MAP fvaGetDeviceMapForImg(const DEVICE_MAP& deviceMap, const QString& pathToFile, QString& deviceName); 

/*!
 * \brief it returns device ID by file name from fva information saved in CSV files
 * \param fvaFile (not full path, just "1212.jpg")
 * \param deviceID - output parameter to contain device ID
 * \returns it returns code of error if any or FVA_NO_ERROR if device ID has been retrieved
 */
FVA_ERROR_CODE fvaGetDeviceIdFromFvaInfo(const QString& fvaFile, int& deviceID);

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

