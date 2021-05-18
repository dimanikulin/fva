/*!
* \file fvacommonlib.h
* \copyright Copyright 2021 FVA Software. All rights reserved. This file is released under the XXX License.
* \author Dima Nikulin.
* \version 0.29
* \date  2014-2021
*/

#ifndef FVACOMMONLIB_H
#define FVACOMMONLIB_H

#include <QtCore/QString>
#include <QtCore/QStringList>
#include <QtCore/QVector>
#include <QtCore/QDateTime>
#include <QtCore/QVariantMap>

#include "data/fvadevice.h"
#include "fvaexitcodes.h"
#include "fvafmtcontext.h"


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
bool fvaIsFVAFile(const QString& extention );

/*!
* \brief it answers if dir is internal one
*/
bool fvaIsInternalDir(const QString& dir);

/*!
 * \brief it tries to parse dir name into period of date
 * \param dirName directory name to be parsed
 * \param from to fill as a date from
 * \param to - to fill as a date to
 * \param ctx - to use formatting options from
 * \returns it returns code of error if any or FVA_NO_ERROR if parsing was successful
 */
FVA_EXIT_CODE fvaParseDirName(const QString& dirName, QDateTime& from, QDateTime& to, const FvaFmtContext& ctx );

/*!
 * \brief it tries to parse file name into date
 * \param fileName file name to be parsed
 * \param date a date to be filled up
 * \param ctx - to use formatting options from
 * \returns it returns code of error if any or FVA_NO_ERROR if parsing was successful
 */
FVA_EXIT_CODE fvaParseFileName(const QString& fileName, QDateTime& date, const FvaFmtContext& ctx);

/*!
 * \brief it returns device id from map loaded
 * \param deviceIds a map to be find in
 * \param pathToFile path to file to get device name from
 * \param deviceName contains device link name
 * \returns it returns map of devices found or empty map if not
 */
DEVICE_MAP fvaGetDeviceMapForImg(const DEVICE_MAP& deviceMap, const QString& pathToFile, QString& deviceName); 

/*!
 * \brief it convert identifiers list from string to int vector
 */
QVector<unsigned int> fvaStringToIds(const QString& strList);

/*!
 * \brief it removes the dir if there is no content inside
 * \param dirPath - path to folder to remove
 * \returns it returns true if folder has been deleted, otherwize it return false
 */
bool fvaRemoveDirIfEmpty(const QString& dirPath);

/*!
 * \brief it creates directory if it does not exist
 * \param dirPath - dir path to create
 * \returns it returns FVA_EXIT_CODE
 */
FVA_EXIT_CODE fvaCreateDirIfNotExists(const QString& dirPath);


#endif // FVACOMMONLIB_H

