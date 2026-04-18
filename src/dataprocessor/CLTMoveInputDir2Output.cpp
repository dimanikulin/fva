/*!
 * \file CLTMoveInputDir2Output.cpp
 * \copyright Copyright 2021 FVA Software. All rights reserved. This file is released under the XXX License.
 * \author Dima Nikulin.
 * \version 0.29
 * \date  2014-2021
 */
#include "CLTMoveInputDir2Output.h"

#include <algorithm>
#include <filesystem>
#include <vector>

#include "fva_qt_port_2_stl.h"
#include "fvacommoncsv.h"

CLTMoveInputDir2Output::CLTMoveInputDir2Output(const FvaConfiguration& cfg) {
    std::string rootSWdir;
    FVA_EXIT_CODE res = cfg.getParamAsString("Common::RootDir", rootSWdir);
    RET_IF_RES_IS_ERROR
    m_rootSWdir = rootSWdir;
}

FVA_EXIT_CODE CLTMoveInputDir2Output::execute(const CLTContext& context) {
    namespace fs = std::filesystem;

    const std::string dirName = m_dir.filename().string();
    fs::path dstDirPath = fs::path(context.outputDir) / dirName.substr(0, std::min<size_t>(4, dirName.size()));

    // skip internal folder
    if (!fvaIsInternalDir(dirName) && !fvaIsInternalDir(toNativePathString(dstDirPath))) {
        // lets create year folder if it does not exist
        FVA_EXIT_CODE res = fvaCreateDirIfNotExists(toNativePathString(dstDirPath));
        if (FVA_ERROR_CANT_CREATE_DIR == res) {
            LOG_CRIT << "could not create dest folder:" << toNativePathString(dstDirPath).c_str();
            return FVA_ERROR_CANT_CREATE_DIR;
        }

        if (dirName.length() == 4)  // YEAR folder
            LOG_DEB << "year folder:" << dirName.c_str();
        else {
            LOG_DEB << "last leaf folder:" << dirName.c_str();
            dstDirPath /= dirName;
        }

        // lets create year/last leaf folder if it does not exist
        res = fvaCreateDirIfNotExists(toNativePathString(dstDirPath));
        if (FVA_ERROR_CANT_CREATE_DIR == res) {
            LOG_CRIT << "could not create dest folder:" << toNativePathString(dstDirPath).c_str();
            return FVA_ERROR_CANT_CREATE_DIR;
        } else if (FVA_ERROR_DEST_DIR_ALREADY_EXISTS == res) {
            // it is first time run
            if (context.custom.empty())
                return FVA_ERROR_DEST_DIR_ALREADY_EXISTS;
            else if (context.custom == "create") {
                LOG_CRIT << "destination dir already exists: " << toNativePathString(dstDirPath).c_str();
                // lets try to create with different name
                dstDirPath += " #1";
                res = fvaCreateDirIfNotExists(toNativePathString(dstDirPath));
                if (FVA_ERROR_CANT_CREATE_DIR == res) {
                    LOG_CRIT << "could not create dest folder:" << toNativePathString(dstDirPath).c_str();
                    return FVA_ERROR_CANT_CREATE_DIR;
                } else if (FVA_ERROR_DEST_DIR_ALREADY_EXISTS == res) {
                    LOG_CRIT << "not immplemented to create dest folder with #2:"
                             << toNativePathString(dstDirPath).c_str();
                    return FVA_ERROR_NOT_IMPLEMENTED;
                } else if (FVA_NO_ERROR == res)
                    LOG_DEB << "created dest folder:" << toNativePathString(dstDirPath).c_str();
            } else if (context.custom == "merge") {
                // do nothing here
            }
        } else if (FVA_NO_ERROR == res)
            LOG_DEB << "created dest folder:" << toNativePathString(dstDirPath).c_str();
    }

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
        const bool isDir = entry.is_directory(entryEc);
        if (entryEc) continue;

        const std::string entryName = entry.path().filename().string();
        const fs::path destinationPath = dstDirPath / entry.path().filename();

        // skip internal folder
        if (isDir && fvaIsInternalDir(entryName)) {
            LOG_WARN << "skipped internal dir : " << entryName.c_str();
            continue;
        }

        // check for already existing
        if (!isDir && fs::exists(destinationPath, entryEc) && !entryEc) {
            LOG_CRIT << "destination file already exists: " << toNativePathString(destinationPath).c_str();
            return FVA_ERROR_DEST_FILE_ALREADY_EXISTS;
        }

        if (context.readOnly == true) {
            if (!isDir) {
                std::error_code renameEc;
                fs::rename(entry.path(), destinationPath, renameEc);
                if (renameEc) {
                    LOG_CRIT << "could not move:" << toNativePathString(entry.path()).c_str() << " into "
                             << toNativePathString(destinationPath).c_str();
                    return FVA_ERROR_CANT_RENAME_FILE;
                }
                LOG_DEB << "moved:" << toNativePathString(entry.path()).c_str() << " into "
                        << toNativePathString(destinationPath).c_str();
            }

            if (isDir && !fvaRemoveDirIfEmpty(toNativePathString(entry.path()))) {
                LOG_CRIT << "could not remove empty source:" << toNativePathString(entry.path()).c_str();
                return FVA_ERROR_CANT_MOVE_DIR;
            } else if (isDir)
                LOG_DEB << "removed empty destination:" << toNativePathString(entry.path()).c_str();
        } else {
            return FVA_ERROR_NOT_IMPLEMENTED;
        }
    }

    return FVA_NO_ERROR;
}