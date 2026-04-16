/*!
 * \file CLTMoveAloneFiles.cpp
 * \copyright Copyright 2021 FVA Software. All rights reserved. This file is released under the XXX License.
 * \author Dima Nikulin.
 * \version 0.29
 * \date  2014-2021
 */
#include "CLTMoveAloneFiles.h"

#include <algorithm>
#include <cctype>
#include <filesystem>
#include <string>
#include <vector>

CLTMoveAloneFiles::CLTMoveAloneFiles(const FvaConfiguration& cfg) {
    FVA_EXIT_CODE res = cfg.getParamAsUint("Rename::minFilesInDir", m_minCountSupportedFiles);
    RET_IF_RES_IS_ERROR
}
FVA_EXIT_CODE CLTMoveAloneFiles::execute(const CLTContext& context) {
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
        return lhs.path().filename().string() < rhs.path().filename().string();
    });

    unsigned int countSupportedFiles = 0;
    for (const auto& entry : entries) {
        std::error_code entryEc;
        if (entry.is_directory(entryEc) || entryEc) continue;

        std::string suffix = entry.path().extension().string();
        if (!suffix.empty() && suffix.front() == '.') suffix.erase(0, 1);
        std::transform(suffix.begin(), suffix.end(), suffix.begin(),
                       [](unsigned char ch) { return static_cast<char>(std::toupper(ch)); });

        FVA_FS_TYPE type = fvaConvertFileExt2FileType(suffix);
        if (FVA_FS_TYPE_UNKNOWN != type) countSupportedFiles++;
    }

    // no need to move these files
    if ((countSupportedFiles >= m_minCountSupportedFiles) || !countSupportedFiles) return FVA_NO_ERROR;

    const std::string dirName = m_dir.filename().string();
    if (dirName.length() == 4) {
        // YEAR folder - no need to move these files
        LOG_DEB << "year folder:" << dirName.c_str();
        return FVA_NO_ERROR;
    }

    const fs::path folderUp = m_dir.parent_path();

    for (const auto& entry : entries) {
        std::error_code entryEc;
        if (entry.is_directory(entryEc) || entryEc) continue;

        const std::string fileName = entry.path().filename().string();

        // skip meta files
        if (fvaIsInternalFile(fileName)) continue;

        const fs::path destination = folderUp / fileName;
        LOG_WARN << "to move to:" << destination.string().c_str() << " from " << entry.path().string().c_str();

        // move the files
        if (!context.readOnly) {
            std::error_code renameEc;
            fs::rename(entry.path(), destination, renameEc);
            if (renameEc) {
                LOG_CRIT << "cant move file:" << entry.path().string().c_str();
                return FVA_ERROR_CANT_RENAME_FILE;
            }
        }
    }
    if (FVA_NO_ERROR == fvaRemoveDirIfEmpty(m_folder)) {
        LOG_WARN << "removed empty dir " << m_folder.c_str();
    }

    return FVA_NO_ERROR;
}
