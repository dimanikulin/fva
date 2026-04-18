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
#include <filesystem>
#include <string>
#include <vector>

#include "fvacommonexif.h"

FVA_EXIT_CODE CLTAutoChecks2::execute(const CLTContext& context) {
    namespace fs = std::filesystem;

    unsigned int countSupportedFiles = 0;
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
            QDateTime from, to;
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
            QDateTime date;
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
                LOG_WARN << "not uniquie file name found: " << fileName.c_str() << ",cur path:"
                         << entryPath.string().c_str() << ",prev path:" << it->second.c_str();
                m_Issues.push_back("FVA_ERROR_NON_UNIQUE_FILE_NAME," + entryPath.string() + "," + fileName);
            } else {
                m_uniqueFileNames[fileName] = entryPath.string();
            }

            if (FVA_FS_TYPE_IMG == type) {
                QDateTime dateTime = fvaGetExifDateTimeOriginalFromFile(
                    QString::fromStdString(entryPath.string()), QString::fromStdString(m_fmtctx.exifDateTime));

                if (!dateTime.isValid()) {
                    LOG_WARN << "empty image taken time found in:" << entryPath.string().c_str();
                    m_Issues.push_back("FVA_ERROR_NULL_TAKEN_TIME," + entryPath.string() + "," + fileName);
                } else if (dateTime != date) {
                    LOG_WARN << "mismatching image taken time found in:" << entryPath.string().c_str();
                    m_Issues.push_back("FVA_ERROR_MISMATCH_TAKEN_TIME," + entryPath.string() + "," + fileName);
                }
            }

            if (FVA_FS_TYPE_VIDEO == type) {
                QDateTime dateTime = fvaGetExifDateTimeOriginalFromFile(
                    QString::fromStdString(entryPath.string()), QString::fromStdString(m_fmtctx.exifDateTime));
                if (!dateTime.isValid()) {
                    LOG_WARN << "empty video taken time found in:" << entryPath.string().c_str();
                    // m_Issues.push_back("FVA_ERROR_EMPTY_VIDEO_TIME," + entryPath.string() + "," + fileName);
                }
            }

            QDateTime dateStart, dateEnd;
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
            if (dateStart == dateEnd) dateEnd = dateEnd.addYears(1);

            QDateTime fileDateTime = QDateTime::fromString(QString::fromStdString(baseFileName),
                                                           QString::fromStdString(m_fmtctx.fvaFileName));
            std::string newFileName = baseFileName;
            size_t pos = 0;
            while ((pos = newFileName.find("##", pos)) != std::string::npos) {
                newFileName.replace(pos, 2, "01");
                pos += 2;
            }
            if (!fileDateTime.isValid()) {
                fileDateTime = QDateTime::fromString(QString::fromStdString(newFileName),
                                                     QString::fromStdString(m_fmtctx.fvaFileName));
            }

            if ((fileDateTime < dateStart) || (fileDateTime > dateEnd)) {
                LOG_CRIT << "unsupported file found:" << entryPath.string().c_str() << " data period=" << dateStart
                         << ";" << dateEnd;
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