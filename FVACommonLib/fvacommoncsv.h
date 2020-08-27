#ifndef FVACOMMONCSV_H
#define FVACOMMONCSV_H

#include "fvaconstants.h"
#include "fvaexitcodes.h"
#include "data\fvapeople.h"

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

#endif // FVACOMMONCSV_H