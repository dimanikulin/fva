/*!
 * \file CLTAutoChecks3.cpp
 * \copyright Copyright 2021 FVA Software. All rights reserved. This file is released under the XXX License.
 * \author Dima Nikulin.
 * \version 0.29
 * \date  2014-2021
 */
#include "CLTAutoChecks3.h"

#include <QtCore/QString>
#include <algorithm>
#include <cctype>
#include <filesystem>
#include <string>
#include <vector>

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
    namespace fs = std::filesystem;

    std::error_code ec;
    std::vector<fs::directory_entry> entries;
    for (const auto& entry : fs::directory_iterator(m_dir, fs::directory_options::skip_permission_denied, ec)) {
        if (ec) {
            LOG_CRIT << "failed to enumerate dir: " << m_folder.c_str();
            return FVA_ERROR_INVALID_ARG;
        }
        entries.push_back(entry);
    }

    std::sort(entries.begin(), entries.end(), [](const fs::directory_entry& lhs, const fs::directory_entry& rhs) {
        std::error_code lhsEc;
        std::error_code rhsEc;
        const bool lhsIsDir = lhs.is_directory(lhsEc);
        const bool rhsIsDir = rhs.is_directory(rhsEc);
        if (lhsIsDir != rhsIsDir) return lhsIsDir > rhsIsDir;
        return lhs.path().filename().string() < rhs.path().filename().string();
    });

    for (const auto& entry : entries) {
        std::error_code entryEc;
        if (entryEc) continue;

        const std::string fileName = entry.path().filename().string();
        std::string fileNameUpper = fileName;
        std::transform(fileNameUpper.begin(), fileNameUpper.end(), fileNameUpper.begin(),
                       [](unsigned char ch) { return static_cast<char>(std::toupper(ch)); });

        std::string suffix = entry.path().extension().string();
        if (!suffix.empty() && suffix.front() == '.') suffix.erase(0, 1);
        std::transform(suffix.begin(), suffix.end(), suffix.begin(),
                       [](unsigned char ch) { return static_cast<char>(std::toupper(ch)); });

        const FVA_FS_TYPE type = entry.is_directory(entryEc) ? FVA_FS_TYPE_UNKNOWN : fvaConvertFileExt2FileType(suffix);
        std::error_code absEc;
        const fs::path absolutePath = fs::absolute(entry.path(), absEc);
        const std::string absoluteFilePath = absEc ? entry.path().string() : absolutePath.string();

        if (fvaIsFVAFile(suffix)) {
            auto it = m_fvaFileInfoC.find(fileNameUpper);
            if (it != m_fvaFileInfoC.end()) m_fvaFileInfoC.erase(it);
        }

        if (FVA_FS_TYPE_IMG != type) continue;

        int deviceID = FVA_UNDEFINED_ID;
        FVA_EXIT_CODE res = fvaGetDeviceIdFromCsv(m_fvaFileInfo, fileName, deviceID);
        if (FVA_NO_ERROR != res) {
            LOG_WARN << "no dev id found for file: " << absoluteFilePath.c_str();
            if (FVA_ERROR_NO_DEV_ID == res)
                m_Issues.push_back("FVA_ERROR_NO_DEV_ID," + absoluteFilePath + "," + fileName);
            if (FVA_ERROR_NON_UNIQUE_FVA_INFO == res)
                m_Issues.push_back("FVA_ERROR_NON_UNIQUE_FVA_INFO," + absoluteFilePath + "," + fileName);
        }

        std::string deviceName;
        DEVICE_MAP devMap = fvaGetDeviceMapForImg(m_deviceMap, entry.path().string(), deviceName);
        if (0 == devMap.size()) {
            LOG_WARN << "unknown device found:" << QString::fromStdString(deviceName).trimmed()
                     << " in file :" << absoluteFilePath.c_str();
            m_Issues.push_back("FVA_ERROR_UNKNOWN_DEVICE," + absoluteFilePath + "," + std::to_string(deviceID) + "," +
                               m_deviceMap[deviceID].guiName + " " + m_deviceMap[deviceID].ownerName);
            if (context.readOnly)
                continue;
            else
                return FVA_ERROR_UNKNOWN_DEVICE;
        }
        if (deviceName.empty()) {
            LOG_WARN << "empty device found:" << QString::fromStdString(deviceName).trimmed()
                     << " in file :" << absoluteFilePath.c_str();
            m_Issues.push_back("FVA_ERROR_EMPTY_DEVICE," + absoluteFilePath + "," + std::to_string(deviceID) + "," +
                               m_deviceMap[deviceID].guiName + " " + m_deviceMap[deviceID].ownerName);
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
            LOG_WARN << "device id linked wrongly, " << absoluteFilePath.c_str() << ",from image-"
                     << devMap.begin()->second.deviceId << ", from fvafile=" << deviceID;
            m_Issues.push_back("FVA_ERROR_LINKED_WRONG_DEVICE," + absoluteFilePath + "," + std::to_string(deviceID) +
                               "," + m_deviceMap[deviceID].guiName + " " + m_deviceMap[deviceID].ownerName);
            continue;
        }

        const bool geoPresent = fvaExifGeoDataPresentInFile(QString::fromStdString(entry.path().string()));
        if (!geoPresent) {
            int placeId = -1;
            auto it = m_fvaFileInfo.find(fileNameUpper);
            if (it != m_fvaFileInfo.end()) {
                placeId = it->second.placeId;
            }

            LOG_WARN << "GEO location is NOT preent in:" << absoluteFilePath.c_str() << ", PlaceId=" << placeId;
            m_Issues.push_back("FVA_ERROR_NO_GEO," + absoluteFilePath + "," + std::to_string(placeId));
        }
    }
    return FVA_NO_ERROR;
}

CLTAutoChecks3::~CLTAutoChecks3() {
    for (auto it = m_fvaFileInfoC.begin(); it != m_fvaFileInfoC.end(); ++it) {
        m_Issues.push_back("FVA_ERROR_NOT_EXISTING_FVA," + it->first);
    }

    fvaSaveStrListToFile(m_rootSWdir + "#logs#/issues3.csv", m_Issues);
}
