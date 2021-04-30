#ifndef FVACOMMONCSV_H
#define FVACOMMONCSV_H

#include "fvaexitcodes.h"

#include "data/fvafile.h"
#include "data/fvadevice.h"
#include "data/fvaperson.h"
#include "data/fvaPeopleRelations.h"

typedef std::map<int, QString> FVA_SIMPLE_MAP;

/*!
 * \brief it returns device ID by file name from fva information saved in CSV files
 * \param fvaFileInfo - fva file information to search in
 * \param fvaFile - file to look information for not full path, just "1212.jpg")
 * \param deviceID - output parameter to contain device ID
 * \returns it returns code of error if any or FVA_NO_ERROR if device ID has been retrieved
 */
FVA_EXIT_CODE fvaGetDeviceIdFromCsv(const FVA_FILE_INFO_MAP& fvaFileInfo, const QString& fvaFile, int& deviceID);

/*!
 * \brief it loads fva information saved in CSV files
 * \param rootSWdir - dir where fva SW is located
 * \param fvaFileInfo - fva file info ti fill up
 * \returns it returns code of error if any or FVA_NO_ERROR if info was loaded
 */
FVA_EXIT_CODE fvaLoadFvaFileInfoFromCsv(const QString& rootSWdir, FVA_FILE_INFO_MAP& fvaFileInfo);

/*!
 * \brief it returns an ID value from file by file path provided
 * \param file - path to file that contains the ID
 * \param int& - variable to fill up by ID loaded from the file
 * \returns it returns code of error if any or FVA_NO_ERROR if loading was successful
 */
FVA_EXIT_CODE fvaGetIDFromFile(const QString& file, int& ID);

/*!
 * \brief it saves an ID value in file by file path provided (it will delete previous content!)
 * \param file - path to file that will contain the ID
 * \param int - ID to save
 * \returns it returns code of error if any or FVA_NO_ERROR if saving was successful
 */
FVA_EXIT_CODE fvaSaveIDInFile(const QString& file, int ID);

/*!
 * \brief it tries to load device map from dictionary
 * \param rootSWdir - dir where fva SW is located
 * \param deviceMap a map to be filled up
 * \returns it returns code of error if any or FVA_NO_ERROR if loading was successful
 */
FVA_EXIT_CODE fvaLoadDeviceMapFromCsv(const QString& rootSWdir, DEVICE_MAP& deviceMap);

/*!
* \brief it tries to load people map from dictionary
* \param rootSWdir - dir where fva SW is located
* \param peopleMap a map to be filled up
* \returns it returns code of error if any or FVA_NO_ERROR if loading was successful
*/
FVA_EXIT_CODE fvaLoadPeopleMapFromCsv(const QString& rootSWdir, PEOPLE_MAP& peopleMap);

/*!
* \brief it tries to load simple map <ID to Name> from dictionary
* \param rootSWdir - dir where fva SW is located
* \param simple map <ID to Name> a map to be filled up
* \param dictName file name of dictionary to load from
* \returns it returns code of error if any or FVA_NO_ERROR if loading was successful
*/
FVA_EXIT_CODE fvaLoadSimpleMapFromCsv(const QString& rootSWdir, FVA_SIMPLE_MAP& simpleMap, const QString& dictName);

/*!
* \brief it tries to load people relations map from dictionary
* \param rootSWdir - dir where FVA SW is located
* \param peopleRelationsMap a map to be filled up
* \returns it returns code of error if any or FVA_NO_ERROR if loading was successful
*/
FVA_EXIT_CODE fvaLoadPeopleRelationMapFromCsv(const QString& rootSWdir, FVA_PEOPLE_RELATION_MAP& peopleRelationsMap);

/*!
* \brief it updates fva dir info with new folder
* \param dirPath - dir path to add
* \returns it returns FVA_EXIT_CODE
*/
FVA_EXIT_CODE fvaUpdateFvaDirInfoInCsv(const QString& dirPath);

#endif // FVACOMMONCSV_H