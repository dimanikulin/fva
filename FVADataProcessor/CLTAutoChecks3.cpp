/*!
* \file CLTAutoChecks3.cpp
* \copyright Copyright 2021 FVA Software. All rights reserved. This file is released under the XXX License.
* \author Dima Nikulin.
* \version 0.29
* \date  2014-2021
*/
#include "CLTAutoChecks3.h"

#include "fvacommoncsv.h"
#include "fvacommonexif.h"

CLTAutoChecks3::CLTAutoChecks3(const FvaConfiguration& cfg)
{
	FVA_EXIT_CODE res = cfg.getParamAsString("Common::RootDir", m_rootSWdir);
	RET_IF_RES_IS_ERROR

	res = fvaLoadFvaFileInfoFromCsv(m_rootSWdir, m_fvaFileInfo);
	RET_IF_RES_IS_ERROR

	m_fvaFileInfoC = m_fvaFileInfo;

	res = fvaLoadDeviceMapFromCsv(m_rootSWdir, m_deviceMap);
	RET_IF_RES_IS_ERROR

	res = m_fmtctx.fillFmtContextFromCfg(cfg);
	RET_IF_RES_IS_ERROR
}
FVA_EXIT_CODE CLTAutoChecks3::execute(const CLTContext& context)
{
	Q_FOREACH(QFileInfo info, m_dir.entryInfoList(QDir::NoDotAndDotDot | QDir::System | QDir::Hidden | QDir::AllDirs | QDir::Files, QDir::DirsFirst))
	{
		QString suffix = info.suffix().toUpper();
		FVA_FS_TYPE type = fvaConvertFileExt2FileType(suffix);

		// remove checked FVA file
		if (fvaIsFVAFile(suffix))
			m_fvaFileInfoC.erase(m_fvaFileInfoC.find(info.fileName().toUpper()));

		if (FVA_FS_TYPE_IMG != type)
			continue;
		//////////////////////////////////// 1. check for exsiting device in fva info by fileName
		int deviceID = FVA_UNDEFINED_ID;
		FVA_EXIT_CODE res = fvaGetDeviceIdFromCsv(m_fvaFileInfo, info.fileName(), deviceID);
		if (FVA_NO_ERROR != res)
		{
			LOG_WARN << "no dev id found for file: " << info.absoluteFilePath();
			if (FVA_ERROR_NO_DEV_ID == res)
				m_Issues.push_back("FVA_ERROR_NO_DEV_ID," + info.absoluteFilePath() + "," + info.fileName());
			if (FVA_ERROR_NON_UNIQUE_FVA_INFO == res)
				m_Issues.push_back("FVA_ERROR_NON_UNIQUE_FVA_INFO," + info.absoluteFilePath() + "," + info.fileName());
		}
		//////////////////////////////////// 2. check for exsiting device in dictionary by device name in pictire
		QString deviceName;
		DEVICE_MAP devMap = fvaGetDeviceMapForImg(m_deviceMap, info.filePath(), deviceName);
		if (0 == devMap.size())
		{
			LOG_WARN << "unknown device found:" << deviceName.trimmed() << " in file :" << info.absoluteFilePath();
			m_Issues.push_back("FVA_ERROR_UKNOWN_DEVICE," + info.absoluteFilePath() + "," + QString::number(deviceID) + "," + m_deviceMap[deviceID].guiName + " " + m_deviceMap[deviceID].ownerName);
			if (context.readOnly)
				continue;
			else
				return FVA_ERROR_UKNOWN_DEVICE;
		}
		if (deviceName.isEmpty())
		{
			LOG_WARN << "empty device found:" << deviceName.trimmed() << " in file :" << info.absoluteFilePath();
			m_Issues.push_back("FVA_ERROR_EMPTY_DEVICE," + info.absoluteFilePath() + "," + QString::number(deviceID) + "," + m_deviceMap[deviceID].guiName + " " + m_deviceMap[deviceID].ownerName);
			continue;
		}
		bool matched = false;
		for (DEVICE_MAP::iterator it = devMap.begin(); it != devMap.end(); ++it)
		{
			if (it.value().deviceId == deviceID)
			{
				matched = true;
				break;
			}
		}

		if (!matched)
		{
			LOG_WARN << "device id linked wrongly, " << info.absoluteFilePath() << ",from image-" << devMap.begin().value().deviceId << ", from fvafile=" << deviceID;
			m_Issues.push_back("FVA_ERROR_LINKED_WRONG_DEVICE," + info.absoluteFilePath() + "," + QString::number(deviceID) + "," + m_deviceMap[deviceID].guiName + " " + m_deviceMap[deviceID].ownerName);
			continue;
		}

		//////////////////////////////////// 3. check for GEO position exsiting in file//////////////////////
		bool GeoPresent = fvaExifGeoDataPresentInFile(info.filePath());
		if (!GeoPresent)
		{
			int PlaceId = -1;
			if (m_fvaFileInfo.find(info.fileName().toUpper()) != m_fvaFileInfo.end())
			{
				PlaceId = m_fvaFileInfo[info.fileName().toUpper()].placeId;
			}

			LOG_WARN << "GEO location is NOT preent in:" << info.absoluteFilePath() << ", PlaceId=" << PlaceId;
			m_Issues.push_back("FVA_ERROR_NO_GEO," + info.absoluteFilePath() + "," + QString::number(PlaceId));
		}

	}
	return FVA_NO_ERROR;
}

CLTAutoChecks3::~CLTAutoChecks3()
{
	for (auto it = m_fvaFileInfoC.begin(); it != m_fvaFileInfoC.end(); ++ it)
	{
		m_Issues.push_back("FVA_ERROR_NOT_EXISTING_FVA," + it.key());
	}

	QFile fileNew(m_rootSWdir + "#logs#/issues3.csv");
	fileNew.open(QIODevice::Append | QIODevice::Text);
	QTextStream writeStream(&fileNew);
	writeStream.setCodec("UTF-8");
	for (auto it = m_Issues.begin(); it != m_Issues.end(); ++it)
		writeStream << *it << "\n";
	writeStream.flush();
	fileNew.close();
}
