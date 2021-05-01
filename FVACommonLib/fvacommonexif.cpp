#include "fvacommonexif.h"
#include "../easyexif/exif.h"
#include <QtCore\QFile>

QString fvaGetExifMakeAndModelFromFile(const QString& pathToFile)
{
	QFile file(pathToFile);
	if (file.open(QIODevice::ReadOnly)){
		QByteArray data = file.readAll(/*1024*1024*/);
		easyexif::EXIFInfo info;
		if (0 == info.parseFrom((unsigned char *)data.data(), data.size()) ){
			return  QString(info.Make.c_str()) + info.Model.c_str();
		}
	}
	return "";
}

QDateTime fvaGetExifDateTimeOriginalFromFile(const QString& pathToFile, const QString& exifDateTimeFmt)
{
	QFile file(pathToFile);
	if (file.open(QIODevice::ReadOnly)){
		QByteArray data = file.readAll(/*1024 * 100*/);
		easyexif::EXIFInfo info;
		if (0== info.parseFrom((unsigned char *)data.data(), data.size())){
			return  QDateTime::fromString(info.DateTimeOriginal.c_str(), exifDateTimeFmt);
		}
	}
	return QDateTime();
}
bool fvaExifGeoDataPresentInFile(const QString& pathToFile)
{
	QFile file(pathToFile);
	if (file.open(QIODevice::ReadOnly)){
		QByteArray data = file.readAll(/*1024 * 100*/);
		easyexif::EXIFInfo info;
		if (0 == info.parseFrom((unsigned char *)data.data(), data.size()))
		{
			easyexif::EXIFInfo::Geolocation_t loc = info.GeoLocation;
			if (loc.Latitude > 0 && loc.Longitude > 0)
				return true;
			else
				return false;
		}
	}
	return false;
}