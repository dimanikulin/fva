/*!
* \file fvacommoncsv.h
* \copyright Copyright 2021 FVA Software. All rights reserved. This file is released under the XXX License.
* \author Dima Nikulin.
* \version 0.29
* \date  2014-2021
*/

#ifndef FVACOMMONCSV_H
#define FVACOMMONCSV_H

#include "fvaexitcodes.h"

#include "fvafile.h"
#include "fvadevice.h"
#include "fvaperson.h"
#include "fvaPeopleRelations.h"

typedef QMap<int, QString> FVA_SIMPLE_MAP;

/*!
 * \brief it returns device ID by file name from fva information saved in CSV files
 * \param fvaFileInfo - fva file information to search in
 * \param fvaFile - file to look information for (not full path, just "1212.jpg")
 * \param deviceID - output parameter to contain device ID
 * \returns it returns code of error if any or FVA_NO_ERROR if device ID has been retrieved
 */
FVA_EXIT_CODE fvaGetDeviceIdFromCsv(const FVA_FILE_INFO_MAP& fvaFileInfo, const QString& fvaFile, int& deviceID);

/*!
 * \brief it loads fva information saved in CSV files
 * \param rootSWdir - dir where fva SW is located
 * \param fvaFileInfo - fva file info ti fill up
 * \param fvaFileName - fva file name to load fva info from
 * \returns it returns code of error if any or FVA_NO_ERROR if info was loaded
 */
FVA_EXIT_CODE fvaLoadFvaFileInfoFromCsv(const QString& rootSWdir, FVA_FILE_INFO_MAP& fvaFileInfo, const QString& fvaFileName = "fvaFile.csv");

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
* \brief it tries to load simple map <ID to Name> from dictionary with predifined type of items
* \param rootSWdir - dir where fva SW is located
* \param simpleMap - <ID to Name> a map to be filled up
* \param dictName file name of dictionary to load from
* \param typeToFilter - a predefined item type to be filtered out from whole scope, if typeToFilter==FVA_UNDEFINED_ID no filtering is applyed
* \returns it returns code of error if any or FVA_NO_ERROR if loading was successful
*/
FVA_EXIT_CODE fvaLoadSimpleMapFromCsvByItemType(const QString& rootSWdir, FVA_SIMPLE_MAP& simpleMap, const QString& dictName, int typeToFilter=FVA_UNDEFINED_ID);

/*!
* \brief it tries to load people relations map from dictionary
* \param rootSWdir - dir where FVA SW is located
* \param peopleRelationsMap a map to be filled up
* \returns it returns code of error if any or FVA_NO_ERROR if loading was successful
*/
FVA_EXIT_CODE fvaLoadPeopleRelationMapFromCsv(const QString& rootSWdir, FVA_PEOPLE_RELATION_MAP& peopleRelationsMap);

/*!
* \brief it loads base item dictionary map from csv
* \param rootSWdir - dir where FVA SW is located
* \param dictMap - a map to be filled up
* \param dictName file name of dictionary to load from
* \returns it returns code of error if any or FVA_NO_ERROR if loading was successful
*/
FVA_EXIT_CODE fvaLoadDictMapFromCsv(const QString& rootSWdir, BASE_DICT_ITEM_MAP& dictMap, const QString& dictName);

#endif // FVACOMMONCSV_H
