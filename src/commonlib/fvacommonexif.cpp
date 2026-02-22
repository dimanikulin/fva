/*!
* \file fvacommonexif.cpp
* \copyright Copyright 2021 FVA Software. All rights reserved. This file is released under the XXX License.
* \author Dima Nikulin.
* \version 0.29
* \date  2014-2021
*/

#include "fvacommonexif.h"
#include "exif.h"
#include <QtCore/QFile>
#include "fvariffparser.h"

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
QDateTime fvaGetVideoTakenTime(const QString& pathToFile, QString& error, const FvaFmtContext& ctx)
{
	QDateTime renameDateTime = fvaGetExifDateTimeOriginalFromFile(pathToFile, ctx.exifDateTime);
	QString _newName = renameDateTime.toString(ctx.fvaFileName);
	if (_newName.isEmpty())
	{
		RiffParser riffInfo;
		QString createdDate;
		if (!riffInfo.open(pathToFile, error) || !riffInfo.findTag("IDIT", createdDate) || !riffInfo.convertToDate(createdDate, renameDateTime, ctx))
			return renameDateTime;
	}
	return renameDateTime;
}
