/*!
 * \file CLTCheckFileFormat.cpp
 * \copyright Copyright 2021 FVA Software. All rights reserved. This file is released under the XXX License.
 * \author Dima Nikulin.
 * \version 0.29
 * \date  2014-2021
 */
#include "CLTCheckFileFormat.h"

#include <algorithm>
#include <cctype>
#include <filesystem>
#include <string>

FVA_EXIT_CODE CLTCheckFileFormat::execute(const CLTContext& context) {
    (void)context;

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

        if (FVA_FS_TYPE_UNKNOWN == fvaConvertFileExt2FileType(suffix)) {
            LOG_CRIT << "found not correct file format:" << entry.path().string().c_str();
            return FVA_ERROR_INCORRECT_FILE_FORMAT;
        }
    }
    return FVA_NO_ERROR;
}
