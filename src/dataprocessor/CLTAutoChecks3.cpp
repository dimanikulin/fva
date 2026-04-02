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

CLTAutoChecks3::CLTAutoChecks3(const FvaConfiguration& cfg) {
    std::string rootSWdir;
    FVA_EXIT_CODE res = cfg.getParamAsString("Common::RootDir", rootSWdir);
    RET_IF_RES_IS_ERROR
    m_rootSWdir = rootSWdir;

    res = fvaLoadFvaFileInfoFromCsv(m_rootSWdir, m_fvaFileInfo);
    RET_IF_RES_IS_ERROR

    m_fvaFileInfoC = m_fvaFileInfo;

    res = fvaLoadDeviceMapFromCsv(m_rootSWdir, m_deviceMap);
    RET_IF_RES_IS_ERROR

    res = m_fmtctx.fillFmtContextFromCfg(cfg);
    RET_IF_RES_IS_ERROR
}
FVA_EXIT_CODE CLTAutoChecks3::execute(const CLTContext& context) {
    Q_FOREACH (QFileInfo info,
               m_dir.entryInfoList(QDir::NoDotAndDotDot | QDir::System | QDir::Hidden | QDir::AllDirs | QDir::Files,
                                   QDir::DirsFirst)) {
        QString suffix = info.suffix().toUpper();
        FVA_FS_TYPE type = fvaConvertFileExt2FileType(suffix.toStdString());

        // remove checked FVA file
        if (fvaIsFVAFile(suffix.toStdString())) {
            auto it = m_fvaFileInfoC.find(info.fileName().toUpper().toStdString());
            if (it != m_fvaFileInfoC.end()) m_fvaFileInfoC.erase(it);
        }

        if (FVA_FS_TYPE_IMG != type) continue;
        //////////////////////////////////// 1. check for exsiting device in fva info by fileName
        int deviceID = FVA_UNDEFINED_ID;
        FVA_EXIT_CODE res = fvaGetDeviceIdFromCsv(m_fvaFileInfo, info.fileName().toStdString(), deviceID);
        if (FVA_NO_ERROR != res) {
            LOG_WARN << "no dev id found for file: " << info.absoluteFilePath();
            if (FVA_ERROR_NO_DEV_ID == res)
                m_Issues.push_back("FVA_ERROR_NO_DEV_ID," + info.absoluteFilePath() + "," + info.fileName());
            if (FVA_ERROR_NON_UNIQUE_FVA_INFO == res)
                m_Issues.push_back("FVA_ERROR_NON_UNIQUE_FVA_INFO," + info.absoluteFilePath() + "," + info.fileName());
        }
        //////////////////////////////////// 2. check for exsiting device in dictionary by device name in pictire
        std::string deviceName;
        DEVICE_MAP devMap = fvaGetDeviceMapForImg(m_deviceMap, info.filePath().toStdString(), deviceName);
        if (0 == devMap.size()) {
            LOG_WARN << "unknown device found:" << QString::fromStdString(deviceName).trimmed() << " in file :"
                     << info.absoluteFilePath();
            m_Issues.push_back("FVA_ERROR_UNKNOWN_DEVICE," + info.absoluteFilePath() + "," + QString::number(deviceID) +
                               "," + QString::fromStdString(m_deviceMap[deviceID].guiName) + " " +
                               QString::fromStdString(m_deviceMap[deviceID].ownerName));
            if (context.readOnly)
                continue;
            else
                return FVA_ERROR_UNKNOWN_DEVICE;
        }
        if (deviceName.empty()) {
            LOG_WARN << "empty device found:" << QString::fromStdString(deviceName).trimmed() << " in file :"
                     << info.absoluteFilePath();
            m_Issues.push_back("FVA_ERROR_EMPTY_DEVICE," + info.absoluteFilePath() + "," + QString::number(deviceID) +
                               "," + QString::fromStdString(m_deviceMap[deviceID].guiName) + " " +
                               QString::fromStdString(m_deviceMap[deviceID].ownerName));
            continue;
        }
        bool matched = false;
        for (DEVICE_MAP::iterator it = devMap.begin(); it != devMap.end(); ++it) {
            if (it->second.deviceId == deviceID) {
                matched = true;
                break;
            }
        }

        if (!matched) {
            LOG_WARN << "device id linked wrongly, " << info.absoluteFilePath() << ",from image-"
                     << devMap.begin()->second.deviceId << ", from fvafile=" << deviceID;
            m_Issues.push_back("FVA_ERROR_LINKED_WRONG_DEVICE," + info.absoluteFilePath() + "," +
                               QString::number(deviceID) + "," + QString::fromStdString(m_deviceMap[deviceID].guiName) +
                               " " + QString::fromStdString(m_deviceMap[deviceID].ownerName));
            continue;
        }

        //////////////////////////////////// 3. check for GEO position exsiting in file//////////////////////
        bool GeoPresent = fvaExifGeoDataPresentInFile(info.filePath());
        if (!GeoPresent) {
            int PlaceId = -1;
            auto it = m_fvaFileInfo.find(info.fileName().toUpper().toStdString());
            if (it != m_fvaFileInfo.end()) {
                PlaceId = it->second.placeId;
            }

            LOG_WARN << "GEO location is NOT preent in:" << info.absoluteFilePath() << ", PlaceId=" << PlaceId;
            m_Issues.push_back("FVA_ERROR_NO_GEO," + info.absoluteFilePath() + "," + QString::number(PlaceId));
        }
    }
    return FVA_NO_ERROR;
}

CLTAutoChecks3::~CLTAutoChecks3() {
    for (auto it = m_fvaFileInfoC.begin(); it != m_fvaFileInfoC.end(); ++it) {
        m_Issues.push_back("FVA_ERROR_NOT_EXISTING_FVA," + QString::fromStdString(it->first));
    }

    std::vector<std::string> issues;
    issues.reserve(m_Issues.size());
    for (const auto& issue : m_Issues) issues.push_back(issue.toStdString());

    fvaSaveStrListToFile(m_rootSWdir + "#logs#/issues3.csv", issues);
}
