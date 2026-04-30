/*!
 * \file CLTCreateDirStructByDevNames.cpp
 * \copyright Copyright 2021 FVA Software. All rights reserved. This file is released under the XXX License.
 * \author Dima Nikulin.
 * \version 0.29
 * \date  2014-2021
 */
#include "CLTCreateDirStructByDevNames.h"

#include <algorithm>
#include <filesystem>
#include <string>
#include <vector>

#include "fva_qt_port_2_stl.h"
#include "fvacommonexif.h"

CLTCreateDirStructByDeviceName::~CLTCreateDirStructByDeviceName() { LOG_DEB << "cmd deleted, dir:" << m_folder.c_str(); }

FVA_EXIT_CODE CLTCreateDirStructByDeviceName::execute(const CLTContext& /*context*/) {
    namespace fs = std::filesystem;

    std::string deviceName;
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
        suffix = fvaStrToUpper(suffix);

        const std::string fullName = toNativePathString(entry.path());
        if (FVA_FS_TYPE_IMG == fvaConvertFileExt2FileType(suffix)) {
            QString newDeviceNameQt = fvaGetExifMakeAndModelFromFile(QString::fromStdString(fullName));

            while (newDeviceNameQt.endsWith('\n')) newDeviceNameQt.chop(1);
            while (newDeviceNameQt.endsWith('\r')) newDeviceNameQt.chop(1);
            while (newDeviceNameQt.endsWith(QChar('\0'))) newDeviceNameQt.chop(1);

            const std::string newDeviceName = trim(newDeviceNameQt.toStdString());

            std::string dirName;
            if (newDeviceName.empty()) {
                LOG_DEB << "no device name in picture:" << fullName.c_str();
                dirName = "EMPTY";
            } else {
                if (deviceName.empty())
                    LOG_DEB << "got first device name: " << newDeviceName.c_str() << "in picture:" << fullName.c_str();
                else if (deviceName != newDeviceName)
                    LOG_WARN << "got new device name: " << newDeviceName.c_str() << "in picture:" << fullName.c_str()
                             << "old: " << deviceName.c_str();

                deviceName = newDeviceName;
                dirName = deviceName;
            }

            const fs::path fullSubFolderPath = m_dir / dirName;

            if (!fs::exists(fullSubFolderPath, entryEc)) {
                fs::create_directory(fullSubFolderPath, entryEc);
                if (entryEc) {
                    LOG_CRIT << "can not create sub-folder:" << toNativePathString(fullSubFolderPath).c_str();
                    return FVA_ERROR_CANT_CREATE_DIR;
                }
                LOG_DEB << "sub-folder:" << dirName.c_str() << " created";
            }

            const fs::path destinationPath = fullSubFolderPath / entry.path().filename();
            fs::rename(entry.path(), destinationPath, entryEc);
            if (entryEc) {
                LOG_CRIT << "can not rename file:" << fullName.c_str()
                         << " into:" << toNativePathString(destinationPath).c_str();
                return FVA_ERROR_CANT_RENAME_FILE;
            }
            LOG_DEB << "file renamed:" << fullName.c_str() << " into:" << toNativePathString(destinationPath).c_str();
        } else
            LOG_DEB << " no device name in:" << fullName.c_str();
    }
    return FVA_NO_ERROR;
}
