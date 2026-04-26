/*!
 * \file CLTCheckLocation.cpp
 * \copyright Copyright 2021 FVA Software. All rights reserved. This file is released under the XXX License.
 * \author Dima Nikulin.
 * \version 0.29
 * \date  2014-2021
 */
#include "CLTCheckLocation.h"

#include <algorithm>
#include <cctype>
#include <filesystem>
#include <string>
#include <vector>

#include "fvacommonexif.h"

CLTCheckLocation::CLTCheckLocation(const FvaConfiguration& cfg) {
    std::string rootSWdir;
    FVA_EXIT_CODE res = cfg.getParamAsString("Common::RootDir", rootSWdir);
    RET_IF_RES_IS_ERROR
    m_rootSWdir = rootSWdir;
}

FVA_EXIT_CODE CLTCheckLocation::execute(const CLTContext& context) {
    namespace fs = std::filesystem;

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
        if (FVA_FS_TYPE_IMG != fvaConvertFileExt2FileType(suffix)) continue;

        const std::string filePath = entry.path().string();
        std::error_code absEc;
        const fs::path absolutePath = fs::absolute(entry.path(), absEc);
        const std::string absoluteFilePath = absEc ? entry.path().string() : absolutePath.string();

        const bool present = fvaExifGeoDataPresentInFile(filePath);
        if (!present) {
            if (context.readOnly) {
                LOG_CRIT << "found file without exif location:" << absoluteFilePath.c_str();
                return FVA_ERROR_NO_EXIF_LOCATION;
            }
            m_Issues.push_back(absoluteFilePath);
        }
    }
    return FVA_NO_ERROR;
}
CLTCheckLocation::~CLTCheckLocation() {
    fvaSaveStrListToFile(m_rootSWdir + "#data#/FVA_ERROR_NO_EXIF_LOCATION.csv", m_Issues);

    LOG_DEB << "cmd deleted, dir:" << m_folder;
}
