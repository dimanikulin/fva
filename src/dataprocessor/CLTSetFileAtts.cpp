/*!
 * \file CLTSetFileAtts.cpp
 * \copyright Copyright 2021 FVA Software. All rights reserved. This file is released under the XXX License.
 * \author Dima Nikulin.
 * \version 0.29
 * \date  2014-2021
 */
#include "CLTSetFileAtts.h"

#include <algorithm>
#include <cctype>
#include <filesystem>
#include <string>

FVA_EXIT_CODE CLTSetFileAtts::execute(const CLTContext& /*context*/) {
    namespace fs = std::filesystem;

    std::error_code ec;
    for (const auto& entry : fs::directory_iterator(m_dir, fs::directory_options::skip_permission_denied, ec)) {
        if (ec) {
            LOG_CRIT << "failed to enumerate dir: " << m_folder.c_str();
            return FVA_ERROR_FAILED_TO_ENUMERATE_DIR;
        }

        std::error_code entryEc;
        if (entry.is_directory(entryEc) || entryEc) continue;

        std::string suffix = entry.path().extension().string();
        if (!suffix.empty() && suffix.front() == '.') suffix.erase(0, 1);
        std::transform(suffix.begin(), suffix.end(), suffix.begin(),
                       [](unsigned char ch) { return static_cast<char>(std::toupper(ch)); });

        const std::string fileName = entry.path().filename().string();
        FVA_FS_TYPE type = fvaConvertFileExt2FileType(suffix);
        if (FVA_FS_TYPE_UNKNOWN == type && !fvaIsInternalFile(fileName)) {
            LOG_CRIT << "found not supported file:" << entry.path().string().c_str();
            return FVA_ERROR_NOT_SUPPORTED_FILE;
        }

        fs::permissions(entry.path(), fs::perms::owner_write | fs::perms::group_write | fs::perms::others_write,
                        fs::perm_options::remove, entryEc);
        if (entryEc) {
            LOG_CRIT << "can not set read-only attr for file:" << entry.path().string().c_str();
            return FVA_ERROR_CANT_SET_PARAM;
        }
    }

    return FVA_NO_ERROR;
}
