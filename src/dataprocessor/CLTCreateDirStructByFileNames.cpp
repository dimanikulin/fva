/*!
 * \file CLTCreateDirStructByFileNames.cpp
 * \copyright Copyright 2021 FVA Software. All rights reserved. This file is released under the XXX License.
 * \author Dima Nikulin.
 * \version 0.29
 * \date  2014-2021
 */
#include "CLTCreateDirStructByFileNames.h"

#include <filesystem>
#include <string>
#include <vector>

#include "fva_qt_port_2_stl.h"

CLTCreateDirStructByFileNames::~CLTCreateDirStructByFileNames() { LOG_DEB << "cmd deleted, dir:" << m_folder; }

FVA_EXIT_CODE CLTCreateDirStructByFileNames::execute(const CLTContext& context) {
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

    for (const auto& entry : entries) {
        std::error_code entryEc;
        if (entry.is_directory(entryEc) || entryEc) continue;

        const std::string fileName = entry.path().filename().string();

        // skip meta files
        if (fvaIsInternalFile(fileName)) continue;

        const std::string baseName = entry.path().stem().string();
        const std::string year = baseName.substr(0, std::min<size_t>(4, baseName.size()));
        fs::path yearFolderPath = m_dir / year;

        if (!fs::exists(yearFolderPath, entryEc)) {
            if (!context.readOnly) fs::create_directory(yearFolderPath, entryEc);
            LOG_DEB << "year-folder:" << toNativePathString(yearFolderPath).c_str() << " created";
        }

        std::string day = baseName.substr(0, std::min<size_t>(10, baseName.size()));
        std::replace(day.begin(), day.end(), '-', '.');
        const fs::path fullSubFolderPath = m_dir / year / day;

        if (!fs::exists(fullSubFolderPath, entryEc)) {
            if (!context.readOnly) fs::create_directory(fullSubFolderPath, entryEc);
            LOG_DEB << "sub-folder:" << toNativePathString(fullSubFolderPath).c_str() << " created";
        }
        if (!context.readOnly) {
            const fs::path destinationPath = fullSubFolderPath / entry.path().filename();
            fs::rename(entry.path(), destinationPath, entryEc);
            if (entryEc) {
                LOG_CRIT << "can not rename file:" << toNativePathString(entry.path()).c_str()
                         << " into:" << toNativePathString(destinationPath).c_str();
                return FVA_ERROR_CANT_RENAME_FILE;
            } else
                LOG_DEB << "file renamed:" << toNativePathString(entry.path()).c_str()
                        << " into:" << toNativePathString(destinationPath).c_str();
        }
    }

    return FVA_NO_ERROR;
}
