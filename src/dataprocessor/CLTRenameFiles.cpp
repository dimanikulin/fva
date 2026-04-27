/*!
 * \file CLTRenameFiles.cpp
 * \copyright Copyright 2021 FVA Software. All rights reserved. This file is released under the XXX License.
 * \author Dima Nikulin.
 * \version 0.29
 * \date  2014-2021
 */
#include "CLTRenameFiles.h"

#include <QtCore/QString>
#include <algorithm>
#include <cctype>
#include <chrono>
#include <filesystem>
#include <string>
#include <vector>

#include "fvacommonexif.h"

namespace {

QDateTime fvaLastWriteTime(const std::filesystem::path& filePath) {
    namespace fs = std::filesystem;

    std::error_code ec;
    const fs::file_time_type fileTime = fs::last_write_time(filePath, ec);
    if (ec) return QDateTime();

    const auto systemTime = std::chrono::time_point_cast<std::chrono::system_clock::duration>(
        fileTime - fs::file_time_type::clock::now() + std::chrono::system_clock::now());
    const std::time_t tt = std::chrono::system_clock::to_time_t(systemTime);
    return QDateTime::fromSecsSinceEpoch(static_cast<qint64>(tt), Qt::UTC).toLocalTime();
}

}  // namespace

CLTRenameFiles::CLTRenameFiles(const FvaConfiguration& cfg) {
    FVA_EXIT_CODE res = cfg.getParamAsBoolean("Rename::videoByModifTime", m_renameVideoByModifTime);
    RET_IF_RES_IS_ERROR

    res = cfg.getParamAsBoolean("Rename::picsByModifTime", m_renamePicsByModifTime);
    RET_IF_RES_IS_ERROR

    res = m_fmtctx.fillFmtContextFromCfg(cfg);
    RET_IF_RES_IS_ERROR
}

bool CLTRenameFiles::checkIfParentFileExist(const std::filesystem::path& fileToCheck, QDateTime& renameDateTime,
                                            const QDateTime& prevRenameDateTime) {
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
                << prevRenameDateTime.addSecs(1).toString(QString::fromStdString(m_fmtctx.fvaFileName));
        renameDateTime = prevRenameDateTime.addSecs(1);
    } else if (baseName.find("_2") != std::string::npos) {
        LOG_DEB << "file contains _2:" << fileToCheck.string().c_str() << ", rename time to use:"
                << prevRenameDateTime.addSecs(2).toString(QString::fromStdString(m_fmtctx.fvaFileName));
        renameDateTime = prevRenameDateTime.addSecs(2);
    }
    return false;
}
void CLTRenameFiles::fillRenameDateTimeFromLastModifiedIfValid(const std::filesystem::path& dirPath,
                                                               const std::filesystem::path& filePath,
                                                               QDateTime& renameDateTime) {
    const std::string dirName = dirPath.filename().string();
    const QString dirDate = QString::fromStdString(dirName.substr(0, std::min<std::size_t>(10, dirName.size())));
    const QDateTime validDateStart = QDateTime::fromString(dirDate, QString::fromStdString(m_fmtctx.fvaDirName));

    if (validDateStart.isValid()) {
        const QDateTime lastModified = fvaLastWriteTime(filePath);
        const QDateTime validDateEnd = validDateStart.addDays(1);
        if ((lastModified > validDateStart) && (lastModified < validDateEnd)) {
            LOG_WARN << " modification time to use for:" << QString::fromStdString(filePath.string())
                     << ",time:" << lastModified.toString(QString::fromStdString(m_fmtctx.fvaFileName));
            renameDateTime = lastModified;
        }
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

    QDateTime renameDateTime;
    QDateTime prevRenameDateTime;

    for (const auto& entry : files) {
        renameDateTime = QDateTime();

        const fs::path entryPath = entry.path();
        const QString absoluteFilePath = QString::fromStdString(entryPath.string());
        const QDateTime lastModified = fvaLastWriteTime(entryPath);

        std::string suffix = entryPath.extension().string();
        if (!suffix.empty() && suffix.front() == '.') suffix.erase(0, 1);
        std::transform(suffix.begin(), suffix.end(), suffix.begin(),
                       [](unsigned char ch) { return static_cast<char>(std::toupper(ch)); });

        const FVA_FS_TYPE fileType = fvaConvertFileExt2FileType(suffix);
        if (FVA_FS_TYPE_IMG == fileType) {
            if (!checkIfParentFileExist(entryPath, renameDateTime, prevRenameDateTime)) {
                renameDateTime =
                    fvaGetExifDateTimeOriginalFromFile(absoluteFilePath, QString::fromStdString(m_fmtctx.exifDateTime));
                const QString maybeName = renameDateTime.toString(QString::fromStdString(m_fmtctx.fvaFileName));
                if (maybeName.isEmpty()) fillRenameDateTimeFromLastModifiedIfValid(m_dir, entryPath, renameDateTime);
                if (!renameDateTime.isValid() && m_renamePicsByModifTime && lastModified.isValid()) {
                    LOG_WARN << "modification time to use (true == FVA_RENAME_FILES_BY_MODIF_TIME_FOR_EMPTY_EXIF) for:"
                             << absoluteFilePath
                             << ", time : " << lastModified.toString(QString::fromStdString(m_fmtctx.fvaFileName));
                    renameDateTime = lastModified;
                }
            }
        } else if (FVA_FS_TYPE_VIDEO == fileType) {
            QString error;
            renameDateTime = fvaGetVideoTakenTime(absoluteFilePath, error, m_fmtctx);
            if (!renameDateTime.isValid()) {
                if (FVA_NO_ERROR != fvaParseFileName(entryPath.stem().string(), renameDateTime, m_fmtctx)) {
                    LOG_WARN << "can not get taken time from:" << absoluteFilePath << ",error:" << error;
                    fillRenameDateTimeFromLastModifiedIfValid(m_dir, entryPath, renameDateTime);
                    if (!renameDateTime.isValid() && m_renameVideoByModifTime && lastModified.isValid()) {
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

        QString newName = renameDateTime.toString(QString::fromStdString(m_fmtctx.fvaFileName));
        if (newName.isEmpty()) {
            LOG_WARN << "no time in img file:" << absoluteFilePath << ",prev time to use:"
                     << prevRenameDateTime.addSecs(1).toString(QString::fromStdString(m_fmtctx.fvaFileName));
            prevRenameDateTime = prevRenameDateTime.addSecs(1);
            newName = prevRenameDateTime.toString(QString::fromStdString(m_fmtctx.fvaFileName));
        } else {
            prevRenameDateTime = renameDateTime;
        }

        if (newName.isEmpty()) {
            LOG_CRIT << "sequence error for file:" << absoluteFilePath;
            return FVA_ERROR_SEQUENCE;
        }

        const std::string extension = entryPath.extension().string();
        const std::string newNameStd = newName.toStdString();
        fs::path newPath;
        if ("OLD_NAME" == context.custom)
            newPath = m_dir / (newNameStd + "__" + entryPath.stem().string() + extension);
        else
            newPath = m_dir / (newNameStd + extension);

        const std::string newPathStr = newPath.string();
        if (context.readOnly) {
            if (m_uniqueFileNames.find(newPathStr) != m_uniqueFileNames.end()) {
                LOG_CRIT << "not unique future file name:" << QString::fromStdString(newPathStr)
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
            LOG_WARN << "file already exists:" << QString::fromStdString(newPathStr)
                     << ",old path:" << absoluteFilePath;
            const fs::path copyDir = m_dir / "#copy";
            if (!fs::exists(copyDir, existsEc)) {
                LOG_WARN << "created COPY folder:" << QString::fromStdString(copyDir.string());
                std::error_code mkdirEc;
                fs::create_directory(copyDir, mkdirEc);
            }
            const fs::path newCopyPath = copyDir / (newNameStd + extension);
            std::error_code renameEc;
            fs::rename(entryPath, newCopyPath, renameEc);
            if (renameEc) {
                LOG_CRIT << "can not rename file:" << absoluteFilePath
                         << " to:" << QString::fromStdString(newCopyPath.string());
                return FVA_ERROR_CANT_RENAME_FILE;
            }
            LOG_DEB << "renamed file:" << absoluteFilePath << " to:" << QString::fromStdString(newCopyPath.string());
            continue;
        }

        std::error_code renameEc;
        fs::rename(entryPath, newPath, renameEc);
        if (renameEc)
            LOG_CRIT << "can not rename file:" << absoluteFilePath << " to:" << QString::fromStdString(newPathStr);
        else
            LOG_DEB << "Renamed:" << absoluteFilePath << " to:" << QString::fromStdString(newPathStr);
    }
    return FVA_NO_ERROR;
}
