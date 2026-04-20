/*!
 * \file CLTCheckDeviceName.cpp
 * \copyright Copyright 2021 FVA Software. All rights reserved. This file is released under the XXX License.
 * \author Dima Nikulin.
 * \version 0.29
 * \date  2014-2021
 */
#include "CLTCheckDeviceName.h"

#include <QtCore/QString>
#include <algorithm>
#include <cctype>
#include <filesystem>
#include <string>
#include <vector>

#include "fvacommonexif.h"

FVA_EXIT_CODE CLTCheckDeviceName::execute(const CLTContext& /*context*/) {
    namespace fs = std::filesystem;

    QString deviceName;
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

    for (const auto& entry : entries) {
        std::error_code entryEc;
        if (entry.is_directory(entryEc) || entryEc) continue;

        std::string suffix = entry.path().extension().string();
        if (!suffix.empty() && suffix.front() == '.') suffix.erase(0, 1);
        std::transform(suffix.begin(), suffix.end(), suffix.begin(),
                       [](unsigned char ch) { return static_cast<char>(std::toupper(ch)); });
        if (FVA_FS_TYPE_IMG != fvaConvertFileExt2FileType(suffix)) continue;

        const QString filePath = QString::fromStdString(entry.path().string());
        std::error_code absEc;
        const fs::path absolutePath = fs::absolute(entry.path(), absEc);
        const QString absoluteFilePath = QString::fromStdString(absEc ? entry.path().string() : absolutePath.string());
        const QString newDeviceName = fvaGetExifMakeAndModelFromFile(filePath);

        if (newDeviceName.isEmpty()) {
            LOG_CRIT << "no device name in picture:" << absoluteFilePath;
            return FVA_ERROR_EMPTY_DEVICE_NAME;
        } else {
            if (deviceName.isEmpty())
                LOG_DEB << "got first device name: " << newDeviceName << "in picture:" << absoluteFilePath;
            else if (deviceName != newDeviceName) {
                LOG_CRIT << "got new device name: " << newDeviceName << "in picture:" << absoluteFilePath
                         << "old: " << deviceName;
                return FVA_ERROR_NON_UNIQUE_DEVICE_NAME;
            }
            deviceName = newDeviceName;
        }
    }

    return FVA_NO_ERROR;
}
