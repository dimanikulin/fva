/*!
 * \file CLTRenameVideoBySequence.cpp
 * \copyright Copyright 2021 FVA Software. All rights reserved. This file is released under the XXX License.
 * \author Dima Nikulin.
 * \version 0.29
 * \date  2014-2021
 */
#include "CLTRenameVideoBySequence.h"

#include <QtCore/QDateTime>
#include <QtCore/QFileInfo>
#include <QtCore/QString>
#include <algorithm>
#include <cctype>
#include <filesystem>
#include <string>
#include <vector>

#include "fvacommonexif.h"

CLTRenameVideoBySequence::CLTRenameVideoBySequence(const FvaConfiguration& cfg) {
    FVA_EXIT_CODE res = cfg.getParamAsBoolean("Rename::videoByModifTime", m_renameVideoByModifTime);
    RET_IF_RES_IS_ERROR

    res = m_fmtctx.fillFmtContextFromCfg(cfg);
    RET_IF_RES_IS_ERROR
}
FVA_EXIT_CODE CLTRenameVideoBySequence::execute(const CLTContext& context) {
    namespace fs = std::filesystem;

    std::string imageFilePrefix;
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
        std::error_code lhsEc;
        std::error_code rhsEc;
        const bool lhsIsDir = lhs.is_directory(lhsEc);
        const bool rhsIsDir = rhs.is_directory(rhsEc);
        if (lhsIsDir != rhsIsDir) return lhsIsDir > rhsIsDir;
        return lhs.path().filename().string() < rhs.path().filename().string();
    });

    for (const auto& entry : entries) {
        std::error_code entryEc;
        if (entry.is_directory(entryEc) || entryEc) continue;

        std::string suffix = entry.path().extension().string();
        if (!suffix.empty() && suffix.front() == '.') suffix.erase(0, 1);
        std::transform(suffix.begin(), suffix.end(), suffix.begin(),
                       [](unsigned char ch) { return static_cast<char>(std::toupper(ch)); });

        const FVA_FS_TYPE type = fvaConvertFileExt2FileType(suffix);
        const std::string baseName = entry.path().stem().string();
        std::error_code absEc;
        const fs::path absolutePath = fs::absolute(entry.path(), absEc);
        const QString absoluteFilePath = QString::fromStdString(absEc ? entry.path().string() : absolutePath.string());

        if (FVA_FS_TYPE_IMG == type && imageFilePrefix.empty()) {
            const std::size_t index = baseName.find('_');
            if (index == std::string::npos) continue;
            imageFilePrefix = baseName.substr(0, index);
            LOG_DEB << "got new image prefix:" << imageFilePrefix.c_str();
        } else if (FVA_FS_TYPE_VIDEO == type) {
            QString error;
            const QString filePath = QString::fromStdString(entry.path().string());
            const QDateTime renameDateTime = fvaGetVideoTakenTime(filePath, error, m_fmtctx);
            if (renameDateTime.isValid() || (!baseName.empty() && baseName.front() == 'P')) {
                continue;
            }

            FVA_EXIT_CODE res = fvaParseFileName(baseName, const_cast<QDateTime&>(renameDateTime), m_fmtctx);
            if (FVA_NO_ERROR == res) {
                continue;
            }
            const std::size_t index = baseName.find('_');
            if (index == std::string::npos) {
                LOG_CRIT << "video file does not contain _:" << absoluteFilePath;
                return FVA_ERROR_SEQUENCE;
            }

            const std::string videoFilePrefix = baseName.substr(0, index);
            if (imageFilePrefix.empty()) {
                QFileInfo info(filePath);
                if (m_renameVideoByModifTime && info.lastModified().isValid()) {
                    continue;
                }
                LOG_CRIT << "still empty image prefix for path:" << absoluteFilePath;
                return FVA_ERROR_SEQUENCE;
            }

            std::string newBaseName = baseName;
            newBaseName.replace(0, videoFilePrefix.size(), imageFilePrefix);
            const fs::path newPath = entry.path().parent_path() / (newBaseName + entry.path().extension().string());
            const QString newFilePath = QString::fromStdString(newPath.string());

            if (absoluteFilePath == newFilePath) {
                LOG_WARN << "file has already target name" << absoluteFilePath << ", skipping";
                continue;
            }

            if (context.readOnly) {
                LOG_DEB << "would rename file:" << absoluteFilePath << " to:" << newFilePath;
                continue;
            }

            std::error_code renameEc;
            fs::rename(entry.path(), newPath, renameEc);
            if (renameEc) {
                LOG_CRIT << "can not rename file:" << absoluteFilePath << " to:" << newFilePath;
                return FVA_ERROR_CANT_RENAME_FILE;
            }
            LOG_DEB << "renamed file:" << absoluteFilePath << " to:" << newFilePath;
        } else if (FVA_FS_TYPE_UNKNOWN == type) {
            LOG_WARN << "unsupported file type:" << absoluteFilePath;
            continue;
        }
    }
    return FVA_NO_ERROR;
}
