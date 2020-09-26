#ifndef FVACOMMONCSV_H
#define FVACOMMONCSV_H

#include "fvaexitcodes.h"

#include "data/fvafile.h"
#include "data/fvadevice.h"
#include "data/fvaperson.h"
#include "data/fvaPeopleRelations.h"

typedef std::map<int, QString> FVA_RELATION_TYPES_MAP;

/*!
 * \brief it returns device ID by file name from fva information saved in CSV files
 * \param fvaFileInfo - fva file information to search in
 * \param fvaFile - file to look information for not full path, just "1212.jpg")
 * \param deviceID - output parameter to contain device ID
 * \param dir - the directory where the file is located
 * \returns it returns code of error if any or FVA_NO_ERROR if device ID has been retrieved
 */
FVA_EXIT_CODE fvaGetDeviceIdFromCsv(const FVA_FILE_INFO_MAP& fvaFileInfo, const QString& fvaFile, int& deviceID, const QString& dir);

/*!
 * \brief it loads fva information saved in CSV files
 * \param - fvaFileInfo - fva file info ti fill up
 * \returns it returns code of error if any or FVA_NO_ERROR if info was loaded
 */
FVA_EXIT_CODE fvaLoadFvaFileInfoFromCsv(FVA_FILE_INFO_MAP& fvaFileInfo);

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
 * \param deviceMap a map to be filled up
 * \returns it returns code of error if any or FVA_NO_ERROR if loading was successful
 */
FVA_EXIT_CODE fvaLoadDeviceMapFromCsv(DEVICE_MAP& deviceMap);

/*!
* \brief it tries to load people map from dictionary
* \param peopleMap a map to be filled up
* \returns it returns code of error if any or FVA_NO_ERROR if loading was successful
*/
FVA_EXIT_CODE fvaLoadPeopleMapFromCsv(PEOPLE_MAP& peopleMap);

/*!
* \brief it tries to load relations type map from dictionary
* \param relationTypesMap a map to be filled up
* \returns it returns code of error if any or FVA_NO_ERROR if loading was successful
*/
FVA_EXIT_CODE fvaLoadRelationTypesMapFromCsv(FVA_RELATION_TYPES_MAP& relationTypesMap);

/*!
* \brief it tries to load people relations map from dictionary
* \param peopleRelationsMap a map to be filled up
* \returns it returns code of error if any or FVA_NO_ERROR if loading was successful
*/
FVA_EXIT_CODE fvaLoadPeopleRelationMapFromCsv(FVA_PEOPLE_RELATION_MAP& peopleRelationsMap);

/*!
* \brief it updates fva dir info with new folder
* \param dirPath - dir path to add
* \returns it returns FVA_EXIT_CODE
*/
FVA_EXIT_CODE fvaUpdateFvaDirInfoInCsv(const QString& dirPath);

#endif // FVACOMMONCSV_H