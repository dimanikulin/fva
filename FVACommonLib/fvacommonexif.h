#ifndef FVACOMMONEXIF_H
#define FVACOMMONEXIF_H

#include <QtCore/QString>
#include <QtCore/QDateTime>

/*!
* \brief it returns Exif camera maker plus model string
* \param pathToFile path file to load camera maker plus model from
* \returns it returns empty string if loading of camera maker and model was not successful, 
* otherwize it returns string that contains <CAMERA MAKER>+<CAMERA MODEL>
*/
QString fvaGetExifMakeAndModelFromFile(const QString& pathToFile);

/*!
* \brief it returns Exif DateTimeOriginal attribute converted to QDateTime
* \param pathToFile path file to load DateTimeOriginal from
* \returns it returns empty QDateTime if loading of DateTimeOriginal not successful, otherwize it returns DateTimeOriginal attribute converted to QDateTime
*/
QDateTime fvaGetExifDateTimeOriginalFromFile(const QString& pathToFile);

/*!
* \brief it checks if Exif GEO Location is present
* \param pathToFile path file to check Exif GEO Location is present from
* \returns it returns true if Exif GEO Location is present, otherwize it returns false 
*/
bool fvaExifGeoDataPresentFromFile(const QString& pathToFile);

#endif // FVACOMMONEXIF_H