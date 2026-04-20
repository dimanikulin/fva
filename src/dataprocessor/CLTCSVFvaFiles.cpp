/*!
 * \file CLTCSVFvaFiles.cpp
 * \copyright Copyright 2021 FVA Software. All rights reserved. This file is released under the XXX License.
 * \author Dima Nikulin.
 * \version 0.29
 * \date  2014-2021
 */
#include "CLTCSVFvaFiles.h"

#include <algorithm>
#include <cctype>
#include <filesystem>
#include <string>
#include <vector>

#include "fvacommoncsv.h"

CLTCSVFvaFile::CLTCSVFvaFile(const FvaConfiguration& cfg) {
    std::string rootSWdir;
    FVA_EXIT_CODE res = cfg.getParamAsString("Common::RootDir", rootSWdir);
    RET_IF_RES_IS_ERROR
    m_rootSWdir = rootSWdir;
}

FVA_EXIT_CODE CLTCSVFvaFile::execute(const CLTContext& context) {
    namespace fs = std::filesystem;

    int ID = FVA_UNDEFINED_ID;
    FVA_EXIT_CODE res = fvaGetIDFromFile(m_rootSWdir + "#data#/fvaFile.id", ID);
    RET_RES_IF_RES_IS_ERROR

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

    std::vector<std::string> records;
    for (const auto& entry : entries) {
        std::error_code entryEc;
        const bool isDir = entry.is_directory(entryEc);
        if (entryEc) continue;

        const std::string fileName = entry.path().filename().string();
        if (isDir && fileName.size() >= 2 && fileName.front() == '#' && fileName.back() == '#') {
            LOG_DEB << "skipped internal fs object - " << entry.path().string().c_str();
            continue;
        }

        if (entry.is_regular_file(entryEc)) {
            std::string suffix = entry.path().extension().string();
            if (!suffix.empty() && suffix.front() == '.') suffix.erase(0, 1);
            std::transform(suffix.begin(), suffix.end(), suffix.begin(),
                           [](unsigned char ch) { return static_cast<char>(std::toupper(ch)); });

            if (!fvaIsFVAFile(suffix)) {
                LOG_DEB << "skipped internal fs object - " << entry.path().string().c_str();
                continue;
            }
        }

        // ID,Name,PlaceId,People,DevId,Description,ScanerId,Comment,EventId,ReasonPeople,reserved1
        std::string csvRecord = std::to_string(++ID) + ","    // ID
                                + fileName + ",,,"            // Name
                                + context.custom + ",,,,,,";  // m_custom here is device id
        records.push_back(csvRecord);
    }
    res = fvaSaveStrListToFile(m_rootSWdir + "#data#/fvaFileN.csv", records);
    if (FVA_NO_ERROR != res) return res;

    return fvaSaveIDInFile(m_rootSWdir + "#data#/fvaFile.id", ID);
}
