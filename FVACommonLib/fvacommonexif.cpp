#include "fvacommonexif.h"

#include "qexifimageheader.h"

QString fvaGetExifMakeAndModelFromFile(const QString& pathToFile)
{
	return QExifImageHeader(pathToFile).value(QExifImageHeader::Make).toString()
	+ QExifImageHeader(pathToFile).value(QExifImageHeader::Model).toString();
}

QDateTime fvaGetExifDateTimeOriginalFromFile(const QString& pathToFile)
{
	return QExifImageHeader().value(QExifImageHeader::DateTimeOriginal).toDateTime();
}