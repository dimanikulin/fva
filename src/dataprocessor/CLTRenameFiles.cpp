/*!
 * \file CLTRenameFiles.cpp
 * \copyright Copyright 2021 FVA Software. All rights reserved. This file is released under the XXX License.
 * \author Dima Nikulin.
 * \version 0.29
 * \date  2014-2021
 */
#include "CLTRenameFiles.h"

#include <algorithm>
#include <cctype>
#include <chrono>
#include <ctime>
#include <filesystem>
#include <iomanip>
#include <sstream>
#include <string>
#include <vector>

#include "fva_qt_port_2_stl.h"
#include "fvacommonexif.h"


CLTRenameFiles::CLTRenameFiles(const FvaConfiguration& cfg) {
    FVA_EXIT_CODE res = cfg.getParamAsBoolean("Rename::videoByModifTime", m_renameVideoByModifTime);
    RET_IF_RES_IS_ERROR

    res = cfg.getParamAsBoolean("Rename::picsByModifTime", m_renamePicsByModifTime);
    RET_IF_RES_IS_ERROR

    res = m_fmtctx.fillFmtContextFromCfg(cfg);
    RET_IF_RES_IS_ERROR
}

bool CLTRenameFiles::checkIfParentFileExist(const std::filesystem::path& fileToCheck,
                                            std::chrono::system_clock::time_point& renameDateTime,
                                            const std::chrono::system_clock::time_point& prevRenameDateTime) {
    std::string newPath = fileToCheck.string();
    const std::size_t idx1 = newPath.rfind("_1");
    if (idx1 != std::string::npos) newPath.erase(idx1, 2);
    const std::size_t idx2 = newPath.rfind("_2");
    if (idx2 != std::string::npos) newPath.erase(idx2, 2);

    std::error_code ec;
    if (!std::filesystem::exists(newPath, ec) || ec) return false;

    const std::string baseName = fileToCheck.stem().string();
    if (baseName.find("_1") != std::string::npos) {
        LOG_DEB << "file contains _1 : " << fileToCheck.string().c_str() << ", rename time to use : "
                << formatDateTime(prevRenameDateTime + std::chrono::seconds(1), m_fmtctx.fvaFileName);
        renameDateTime = prevRenameDateTime + std::chrono::seconds(1);
    } else if (baseName.find("_2") != std::string::npos) {
        LOG_DEB << "file contains _2:" << fileToCheck.string().c_str() << ", rename time to use:"
                << formatDateTime(prevRenameDateTime + std::chrono::seconds(2), m_fmtctx.fvaFileName);
        renameDateTime = prevRenameDateTime + std::chrono::seconds(2);
    }
    return false;
}
void CLTRenameFiles::fillRenameDateTimeFromLastModifiedIfValid(const std::filesystem::path& dirPath,
                                                               const std::filesystem::path& filePath,
                                                               std::chrono::system_clock::time_point& renameDateTime) {
    const std::string dirName = dirPath.filename().string();
    const std::string dirDate = dirName.substr(0, std::min<std::size_t>(10, dirName.size()));
    std::tm parsedDate = {};
    if (!parseDateTime(dirDate, m_fmtctx.fvaDirName, parsedDate)) return;

    const auto validDateStart = fromStdTm(parsedDate);
    if (validDateStart == std::chrono::system_clock::time_point{}) return;

    const auto lastModified = fvaLastWriteTime(filePath);
    const auto validDateEnd = validDateStart + std::chrono::hours(24);
    if ((lastModified > validDateStart) && (lastModified < validDateEnd)) {
        LOG_WARN << " modification time to use for:" << filePath.string()
                 << ",time:" << formatDateTime(lastModified, m_fmtctx.fvaFileName);
        renameDateTime = lastModified;
    }
}

FVA_EXIT_CODE CLTRenameFiles::execute(const CLTContext& context) {
    namespace fs = std::filesystem;

    std::error_code ec;
    std::vector<fs::directory_entry> files;
    for (const auto& entry : fs::directory_iterator(m_dir, fs::directory_options::skip_permission_denied, ec)) {
        if (ec) {
            LOG_CRIT << "failed to enumerate dir: " << m_folder.c_str();
            return FVA_ERROR_FAILED_TO_ENUMERATE_DIR;
        }

        std::error_code entryEc;
        if (entry.is_regular_file(entryEc) && !entryEc) files.push_back(entry);
    }

    std::sort(files.begin(), files.end(), [](const fs::directory_entry& lhs, const fs::directory_entry& rhs) {
        return lhs.path().filename().string() < rhs.path().filename().string();
    });

    std::chrono::system_clock::time_point renameDateTime;
    std::chrono::system_clock::time_point prevRenameDateTime;

    for (const auto& entry : files) {
        renameDateTime = std::chrono::system_clock::time_point{};

        const fs::path entryPath = entry.path();
        const std::string absoluteFilePath = entryPath.string();
        const auto lastModified = fvaLastWriteTime(entryPath);

        std::string suffix = entryPath.extension().string();
        if (!suffix.empty() && suffix.front() == '.') suffix.erase(0, 1);
        std::transform(suffix.begin(), suffix.end(), suffix.begin(),
                       [](unsigned char ch) { return static_cast<char>(std::toupper(ch)); });

        const FVA_FS_TYPE fileType = fvaConvertFileExt2FileType(suffix);
        if (FVA_FS_TYPE_IMG == fileType) {
            if (!checkIfParentFileExist(entryPath, renameDateTime, prevRenameDateTime)) {
                renameDateTime = fvaGetExifDateTimeOriginalFromFile(absoluteFilePath, m_fmtctx.exifDateTime);
                const std::string maybeName = formatDateTime(renameDateTime, m_fmtctx.fvaFileName);
                if (maybeName.empty()) fillRenameDateTimeFromLastModifiedIfValid(m_dir, entryPath, renameDateTime);
                if (renameDateTime == std::chrono::system_clock::time_point{} && m_renamePicsByModifTime &&
                    lastModified != std::chrono::system_clock::time_point{}) {
                    LOG_WARN << "modification time to use (true == FVA_RENAME_FILES_BY_MODIF_TIME_FOR_EMPTY_EXIF) for:"
                             << absoluteFilePath
                             << ", time : " << formatDateTime(lastModified, m_fmtctx.fvaFileName);
                    renameDateTime = lastModified;
                }
            }
        } else if (FVA_FS_TYPE_VIDEO == fileType) {
            std::string error;
            renameDateTime = fvaGetVideoTakenTime(absoluteFilePath, error, m_fmtctx);
            if (renameDateTime == std::chrono::system_clock::time_point{}) {
                std::tm parsedDate = {};
                if (FVA_NO_ERROR == fvaParseFileName(entryPath.stem().string(), parsedDate, m_fmtctx)) {
                    renameDateTime = fromStdTm(parsedDate);
                } else {
                    LOG_WARN << "can not get taken time from:" << absoluteFilePath << ",error:" << error;
                    fillRenameDateTimeFromLastModifiedIfValid(m_dir, entryPath, renameDateTime);
                    if (renameDateTime == std::chrono::system_clock::time_point{} && m_renameVideoByModifTime &&
                        lastModified != std::chrono::system_clock::time_point{}) {
                        LOG_WARN << "last modif time to use (FVA_RENAME_VIDEO_BY_MODIF_TIME_IF_EMPTY_EXIF == true): for"
                                 << absoluteFilePath;
                        renameDateTime = lastModified;
                    }
                }
            }
        } else if (FVA_FS_TYPE_AUDIO == fileType) {
            fillRenameDateTimeFromLastModifiedIfValid(m_dir, entryPath, renameDateTime);
        } else {
            LOG_WARN << "unsupported file type:" << absoluteFilePath;
            continue;
        }

        std::string newName = formatDateTime(renameDateTime, m_fmtctx.fvaFileName);
        if (newName.empty()) {
            LOG_WARN << "no time in img file:" << absoluteFilePath << ",prev time to use:"
                     << formatDateTime(prevRenameDateTime + std::chrono::seconds(1), m_fmtctx.fvaFileName);
            prevRenameDateTime = prevRenameDateTime + std::chrono::seconds(1);
            newName = formatDateTime(prevRenameDateTime, m_fmtctx.fvaFileName);
        } else {
            prevRenameDateTime = renameDateTime;
        }

        if (newName.empty()) {
            LOG_CRIT << "sequence error for file:" << absoluteFilePath;
            return FVA_ERROR_SEQUENCE;
        }

        const std::string extension = entryPath.extension().string();
        fs::path newPath;
        if ("OLD_NAME" == context.custom)
            newPath = m_dir / (newName + "__" + entryPath.stem().string() + extension);
        else
            newPath = m_dir / (newName + extension);

        const std::string newPathStr = newPath.string();
        if (context.readOnly) {
            if (m_uniqueFileNames.find(newPathStr) != m_uniqueFileNames.end()) {
                LOG_CRIT << "not unique future file name:" << newPathStr
                         << " for file: " << absoluteFilePath;
                return FVA_ERROR_NON_UNIQUE_FILE_NAME;
            }
            m_uniqueFileNames[newPathStr] = entryPath.filename().string();
            continue;
        }

        if (entryPath == newPath) {
            LOG_WARN << "File already has the same name:" << absoluteFilePath;
            continue;
        }

        std::error_code existsEc;
        if (fs::exists(newPath, existsEc) && !existsEc) {
            LOG_WARN << "file already exists:" << newPathStr
                     << ",old path:" << absoluteFilePath;
            const fs::path copyDir = m_dir / "#copy";
            if (!fs::exists(copyDir, existsEc)) {
                LOG_WARN << "created COPY folder:" << copyDir.string();
                std::error_code mkdirEc;
                fs::create_directory(copyDir, mkdirEc);
            }
            const fs::path newCopyPath = copyDir / (newName + extension);
            std::error_code renameEc;
            fs::rename(entryPath, newCopyPath, renameEc);
            if (renameEc) {
                LOG_CRIT << "can not rename file:" << absoluteFilePath
                         << " to:" << newCopyPath.string();
                return FVA_ERROR_CANT_RENAME_FILE;
            }
            LOG_DEB << "renamed file:" << absoluteFilePath << " to:" << newCopyPath.string();
            continue;
        }

        std::error_code renameEc;
        fs::rename(entryPath, newPath, renameEc);
        if (renameEc)
            LOG_CRIT << "can not rename file:" << absoluteFilePath << " to:" << newPathStr;
        else
            LOG_DEB << "Renamed:" << absoluteFilePath << " to:" << newPathStr;
    }
    return FVA_NO_ERROR;
}
