/*!
 * \file fvacommonexif.h
 * \copyright Copyright 2021 FVA Software. All rights reserved. This file is released under the XXX License.
 * \author Dima Nikulin.
 * \version 0.29
 * \date  2014-2021
 */

#ifndef FVACOMMONEXIF_H
#define FVACOMMONEXIF_H

#include <chrono>
#include <string>

#include "fvafmtcontext.h"

/*!
 * \brief it returns Exif camera maker plus model string
 * \param pathToFile path file to load camera maker plus model from
 * \returns it returns empty string if loading of camera maker and model was not successful,
 * otherwize it returns string that contains <CAMERA MAKER>+<CAMERA MODEL>
 */
std::string fvaGetExifMakeAndModelFromFile(const std::string& pathToFile);

/*!
 * \brief it returns Exif DateTimeOriginal attribute converted to std::chrono time_point
 * \param pathToFile path file to load DateTimeOriginal from
 * \param exifDateTimeFmt format of an EXIF date
 * \returns it returns default-constructed time_point if loading of DateTimeOriginal not successful, otherwize it
 * returns DateTimeOriginal attribute converted to std::chrono::system_clock::time_point
 */
std::chrono::system_clock::time_point fvaGetExifDateTimeOriginalFromFile(const std::string& pathToFile,
                                                                         const std::string& exifDateTimeFmt);

/*!
 * \brief it checks if Exif GEO Location is present
 * \param pathToFile path file to check Exif GEO Location is present from
 * \returns it returns true if Exif GEO Location is present, otherwize it returns false
 */
bool fvaExifGeoDataPresentInFile(const std::string& pathToFile);

/*!
 * \brief it tries to get time a video taken at from header
 * \param pathToFile a path to video file
 * \param error error description
 * \param ctx - to use formatting options from
 * \returns it returns time taken
 */
std::chrono::system_clock::time_point fvaGetVideoTakenTime(const std::string& pathToFile, std::string& error,
                                                           const FvaFmtContext& ctx);

#endif  // FVACOMMONEXIF_H
