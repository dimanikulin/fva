/*!
 * \file CmdLineBaseTask.cpp
 * \copyright Copyright 2021 FVA Software. All rights reserved. This file is released under the XXX License.
 * \author Dima Nikulin.
 * \version 0.29
 * \date  2014-2021
 */
#include "CmdLineBaseTask.h"

#include <algorithm>
#include <vector>

FVA_EXIT_CODE CmdLineBaseTask::processFolderRecursivly(const std::string& folder, const CLTContext& context) {
    namespace fs = std::filesystem;

    const fs::path rootPath(folder);
    std::error_code ec;
    std::vector<fs::directory_entry> entries;

    if (fs::exists(rootPath, ec) && fs::is_directory(rootPath, ec)) {
        for (const auto& entry : fs::directory_iterator(rootPath, fs::directory_options::skip_permission_denied, ec)) {
            if (ec) {
                LOG_WARN << "failed to enumerate dir - " << folder.c_str();
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

        for (const auto& entry : entries) {
            std::error_code entryEc;
            if (!entry.is_directory(entryEc) || entryEc) continue;

            const std::string entryName = entry.path().filename().string();
            if (entryName.size() >= 2 && entryName.front() == '#' && entryName.back() == '#') {
                LOG_DEB << "skipped internal dir - " << entry.path().string().c_str();
                continue;
            }

            const std::string entryPath = toNativePathString(entry.path());
            m_folder = entryPath;
            m_dir = entry.path();

            FVA_EXIT_CODE res = processFolderRecursivly(entryPath, context);
            RET_RES_IF_RES_IS_ERROR
        }
    }

    m_folder = toNativePathString(rootPath);
    m_dir = rootPath;

    return execute(context);
}
