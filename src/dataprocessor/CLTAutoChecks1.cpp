/*!
 * \file CLTAutoChecks1.cpp
 * \copyright Copyright 2021 FVA Software. All rights reserved. This file is released under the XXX License.
 * \author Dima Nikulin.
 * \version 0.29
 * \date  2014-2021
 */
#include "CLTAutoChecks1.h"

#include <algorithm>
#include <filesystem>
#include <vector>

#include "fva_qt_port_2_stl.h"
#include "fvacommonexif.h"

CLTAutoChecks1::CLTAutoChecks1(const FvaConfiguration& cfg) {
    FVA_EXIT_CODE res = cfg.getParamAsBoolean("Rename::videoByModifTime", m_renameVideoByModifTime);
    RET_IF_RES_IS_ERROR

    res = m_fmtctx.fillFmtContextFromCfg(cfg);
    RET_IF_RES_IS_ERROR
}

FVA_EXIT_CODE CLTAutoChecks1::execute(const CLTContext& /*context*/) {
    namespace fs = std::filesystem;

    std::error_code ec;
    std::vector<fs::directory_entry> entries;

    for (const auto& entry : fs::directory_iterator(m_dir, fs::directory_options::skip_permission_denied, ec)) {
        if (ec) {
            LOG_WARN << "failed to enumerate dir - " << m_folder.c_str();
            break;
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

    bool first = false;
    for (const auto& entry : entries) {
        std::error_code entryEc;
        if (entry.is_directory(entryEc) || entryEc) continue;

        const std::string path = toNativePathString(entry.path());
        const std::string fileName = entry.path().filename().string();

        std::string suffix = entry.path().extension().string();
        if (!suffix.empty() && suffix.front() == '.') suffix.erase(0, 1);
        suffix = toUpper(suffix);

        FVA_FS_TYPE type = fvaConvertFileExt2FileType(suffix);

        //	#01.NotVideoFirst
        if (!first) {
            first = true;
            if (FVA_FS_TYPE_VIDEO == type || FVA_FS_TYPE_AUDIO == type) {
                QString error;
                QDateTime time = fvaGetVideoTakenTime(QString::fromStdString(path), error, m_fmtctx);
                if (!time.isValid()) {
                    const auto writeTime = entry.last_write_time(entryEc);
                    const bool hasLastWriteTime = !entryEc && (writeTime != fs::file_time_type::min());
                    if (m_renameVideoByModifTime == true && hasLastWriteTime) {
                        LOG_WARN
                            << "skipping first video/audio file (FVA_RENAME_VIDEO_BY_MODIF_TIME_IF_EMPTY_EXIF == true):"
                            << path.c_str();
                        continue;
                    }
                    LOG_CRIT << "found first video/audio file:" << path.c_str();
                    return FVA_ERROR_VIDEO_FIRST;
                }
            }
        }

        // #02.NotSTFiles
        const std::string st = fileName.size() >= 2 ? toUpper(fileName.substr(0, 2)) : std::string();
        if (st == "ST") {
            LOG_CRIT << "found panoram file:" << path.c_str();
            return FVA_ERROR_PANORAM_FILE;
        }
    }

    return FVA_NO_ERROR;
}