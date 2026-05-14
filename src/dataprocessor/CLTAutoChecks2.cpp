/*!
 * \file CLTAutoChecks2.cpp
 * \copyright Copyright 2021 FVA Software. All rights reserved. This file is released under the XXX License.
 * \author Dima Nikulin.
 * \version 0.29
 * \date  2014-2021
 */
#include "CLTAutoChecks2.h"

#include <algorithm>
#include <cctype>
#include <chrono>
#include <ctime>
#include <filesystem>
#include <iomanip>
#include <sstream>
#include <string>
#include <vector>

#include "fvacommonexif.h"

namespace {

std::string formatTm(const std::tm& value) {
    std::ostringstream oss;
    oss << std::put_time(&const_cast<std::tm&>(value), "%Y-%m-%d %H:%M:%S");
    return oss.str();
}

std::time_t toTimeT(std::tm value) {
    value.tm_isdst = -1;
    return std::mktime(&value);
}

bool sameDateTime(const std::chrono::system_clock::time_point& lhs, const std::tm& rhs) {
    time_t lhsTime = std::chrono::system_clock::to_time_t(lhs);
    time_t rhsTime = toTimeT(rhs);
    return std::difftime(lhsTime, rhsTime) == 0.0;
}

}  // namespace

FVA_EXIT_CODE CLTAutoChecks2::execute(const CLTContext& context) {
    namespace fs = std::filesystem;

    unsigned int countSupportedFiles = 0;
    std::error_code ec;
    std::vector<fs::directory_entry> entries;
    for (const auto& entry : fs::directory_iterator(m_dir, fs::directory_options::skip_permission_denied, ec)) {
        if (ec) {
            LOG_CRIT << "failed to enumerate dir: " << m_folder.c_str();
            return FVA_ERROR_FAILED_TO_ENUMERATE_DIR;
        }
        entries.push_back(entry);
    }

    std::sort(entries.begin(), entries.end(), [](const fs::directory_entry& lhs, const fs::directory_entry& rhs) {
        return lhs.path().filename().string() < rhs.path().filename().string();
    });

    for (const auto& entry : entries) {
        std::error_code entryEc;
        const bool isDir = entry.is_directory(entryEc);
        if (entryEc) continue;

        const fs::path entryPath = entry.path();
        const std::string fileName = entryPath.filename().string();

        // 1.CHECK FOR PROPER FOLDER NAME (NO COPY FOLDER)
        if (isDir) {
            std::tm from = {};
            std::tm to = {};
            if (FVA_NO_ERROR != fvaParseDirName(fileName, from, to, m_fmtctx)) {
                if (fvaIsInternalDir(fileName)) continue;

                LOG_CRIT << "wrong folder name:" << entryPath.string().c_str();
                m_Issues.push_back("FVA_ERROR_WRONG_FOLDER_NAME," + entryPath.string() + "," + fileName);
                if (context.readOnly)
                    continue;
                else
                    return FVA_ERROR_WRONG_FOLDER_NAME;
            }
            continue;
        }

        // 2.CHECK FOR PROPER FILE NAME AND SUPPORTED TYPE
        std::string suffix = entryPath.extension().string();
        if (!suffix.empty() && suffix.front() == '.') suffix.erase(0, 1);
        std::transform(suffix.begin(), suffix.end(), suffix.begin(),
                       [](unsigned char ch) { return static_cast<char>(std::toupper(ch)); });

        FVA_FS_TYPE type = fvaConvertFileExt2FileType(suffix);
        if (FVA_FS_TYPE_UNKNOWN != type) {
            countSupportedFiles++;  // it is our file
            std::tm date = {};
            const std::string baseFileName = entryPath.stem().string();
            if (FVA_NO_ERROR != fvaParseFileName(baseFileName, date, m_fmtctx)) {
                LOG_CRIT << "unsupported file found:" << entryPath.string().c_str();
                m_Issues.push_back("FVA_ERROR_WRONG_FILE_NAME," + entryPath.string() + "," + fileName);
                if (context.readOnly)
                    continue;
                else
                    return FVA_ERROR_WRONG_FILE_NAME;
            }

            auto it = m_uniqueFileNames.find(fileName);
            if (m_uniqueFileNames.end() != it) {
                LOG_WARN << "not uniquie file name found: " << fileName.c_str()
                         << ",cur path:" << entryPath.string().c_str() << ",prev path:" << it->second.c_str();
                m_Issues.push_back("FVA_ERROR_NON_UNIQUE_FILE_NAME," + entryPath.string() + "," + fileName);
            } else {
                m_uniqueFileNames[fileName] = entryPath.string();
            }

            if (FVA_FS_TYPE_IMG == type) {
                const auto dateTime = fvaGetExifDateTimeOriginalFromFile(entryPath.string(), m_fmtctx.exifDateTime);

                if (dateTime == std::chrono::system_clock::time_point{}) {
                    LOG_WARN << "empty image taken time found in:" << entryPath.string().c_str();
                    m_Issues.push_back("FVA_ERROR_NULL_TAKEN_TIME," + entryPath.string() + "," + fileName);
                } else if (!sameDateTime(dateTime, date)) {
                    LOG_WARN << "mismatching image taken time found in:" << entryPath.string().c_str();
                    m_Issues.push_back("FVA_ERROR_MISMATCH_TAKEN_TIME," + entryPath.string() + "," + fileName);
                }
            }

            if (FVA_FS_TYPE_VIDEO == type) {
                const auto dateTime = fvaGetExifDateTimeOriginalFromFile(entryPath.string(), m_fmtctx.exifDateTime);
                if (dateTime == std::chrono::system_clock::time_point{}) {
                    LOG_WARN << "empty video taken time found in:" << entryPath.string().c_str();
                    // m_Issues.push_back("FVA_ERROR_EMPTY_VIDEO_TIME," + entryPath.string() + "," + fileName);
                }
            }

            std::tm dateStart = {};
            std::tm dateEnd = {};
            const std::string dirName = m_dir.filename().string();
            if (FVA_NO_ERROR != fvaParseDirName(dirName, dateStart, dateEnd, m_fmtctx)) {
                if (fvaIsInternalDir(dirName)) continue;

                LOG_CRIT << "wrong matching folder name:" << entryPath.string().c_str();
                m_Issues.push_back("FVA_ERROR_WRONG_FOLDER_NAME," + entryPath.string() + "," + fileName);
                if (context.readOnly)
                    continue;
                else
                    return FVA_ERROR_WRONG_FOLDER_NAME;
            }
            if (std::difftime(toTimeT(dateStart), toTimeT(dateEnd)) == 0.0) {
                dateEnd.tm_year += 1;
                dateEnd.tm_isdst = -1;
                std::mktime(&dateEnd);
            }

            const std::time_t fileTime = toTimeT(date);
            const std::time_t dateStartTime = toTimeT(dateStart);
            const std::time_t dateEndTime = toTimeT(dateEnd);

            if ((fileTime < dateStartTime) || (fileTime > dateEndTime)) {
                LOG_CRIT << "unsupported file found:" << entryPath.string().c_str()
                         << " data period=" << formatTm(dateStart) << ";" << formatTm(dateEnd);
                m_Issues.push_back("FVA_ERROR_NOT_SUPPORTED_FILE," + entryPath.string() + "," + fileName);
                if (context.readOnly)
                    continue;
                else
                    return FVA_ERROR_NOT_SUPPORTED_FILE;
            }
            countSupportedFiles++;
        } else if (fvaIsInternalFile(fileName)) {
            m_Issues.push_back("FVA_ERROR_INTERNAL_FILE," + entryPath.string() + "," + fileName);
        } else {
            LOG_CRIT << "unsupported file found:" << entryPath.string().c_str();
            m_Issues.push_back("FVA_ERROR_NOT_SUPPORTED_FILE," + entryPath.string() + "," + fileName);
            if (!context.readOnly) return FVA_ERROR_NOT_SUPPORTED_FILE;
        }
    }

    m_fileCount[m_folder] = countSupportedFiles;
    if ((countSupportedFiles < m_minCountSupportedFiles) && countSupportedFiles) {
        const std::string dirName = m_dir.filename().string();
        if (dirName.length() == 4) {
            LOG_DEB << "year folder:" << dirName.c_str() << " skipping check for too little supported files";
        } else {
            LOG_CRIT << "too little supported files found in:" << m_folder.c_str();
            m_Issues.push_back("FVA_ERROR_TOO_LITTLE_FILES," + m_folder + ",");
            if (!context.readOnly) return FVA_ERROR_TOO_LITTLE_FILES;
        }
    }
    return FVA_NO_ERROR;
}
std::map<unsigned int, unsigned int> sizes;

CLTAutoChecks2::CLTAutoChecks2(const FvaConfiguration& cfg) {
    FVA_EXIT_CODE res = cfg.getParamAsString("Common::RootDir", m_rootSWdir);
    RET_IF_RES_IS_ERROR

    res = cfg.getParamAsUint("Rename::minFilesInDir", m_minCountSupportedFiles);
    RET_IF_RES_IS_ERROR

    res = m_fmtctx.fillFmtContextFromCfg(cfg);
    RET_IF_RES_IS_ERROR
}
CLTAutoChecks2::~CLTAutoChecks2() {
    LOG_DEB << "cmd deleted, dir:" << m_folder.c_str();
    for (auto it = m_fileCount.begin(); it != m_fileCount.end(); ++it) {
        sizes[it->second] = ++sizes[it->second];
    }
    if (m_Issues.empty()) return;  // not to create file for no issues

    fvaSaveStrListToFile(m_rootSWdir + "#logs#/issues2.csv", m_Issues);
}
