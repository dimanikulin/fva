#ifndef FVADEFAULTCFG_H
#define FVADEFAULTCFG_H

#include <QtCore/Qstring>

const QString	FVA_DEFAULT_ROOT_DIR				= "C:/FVA/";

/*!
 * \brief how many supported files should be in a folder
 */
const unsigned int FVA_DEFAULT_MIN_COUNT_FILES_IN_DIR	= 0;

////////////////////////// RENAMING FILE OPTIONS   //////////////////////////////////////////
/*!
* \brief shall be renamed pictures files using the file modification time if they do not have exif taken time set
*/
const bool FVA_RENAME_PICS_BY_MODIF_TIME_IF_EMPTY_EXIF = true;

/*!
* \brief shall be renamed video files using the file modification time if they do not have taken time set
*/
const bool FVA_RENAME_VIDEO_BY_MODIF_TIME_IF_EMPTY_EXIF = true;

////////////////////////// FORMATTING STRING OPTIONS/////////////////////////////////////////
/*!
 * \brief format a dir name as a date
 */
const QString FVA_DIR_NAME_FMT = "yyyy.MM.dd";

/*!
 * \brief format a dir name as a year
 */
const QString FVA_DIR_NAME_YEAR_FMT = "yyyy";

/*!
 * \brief format a FVA file name as a date + time
 */
const QString FVA_FILE_NAME_FMT = "yyyy-MM-dd-hh-mm-ss";

/*!
* \brief format an EXIF date + time
*/
const QString EXIF_DATE_TIME_FMT = "yyyy:MM:dd hh:mm:ss";

#endif // FVADEFAULTCFG_H
