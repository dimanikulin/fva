/*!
 * \file CLTFixDuplicatedFileNames.cpp
 * \copyright Copyright 2021 FVA Software. All rights reserved. This file is released under the XXX License.
 * \author Dima Nikulin.
 * \version 0.29
 * \date  2014-2021
 */
#include "CLTFixDuplicatedFileNames.h"

#include <cctype>
#include <filesystem>

// TODO to work with digiKam dir but not fva dir
CLTFixDuplicatedFileNames::CLTFixDuplicatedFileNames(const FvaConfiguration& cfg) {
    FVA_EXIT_CODE res = cfg.getParamAsString("Common::RootDir", m_rootSWdir);
    RET_IF_RES_IS_ERROR
}

FVA_EXIT_CODE CLTFixDuplicatedFileNames::execute(const CLTContext& context) {
    namespace fs = std::filesystem;

    const std::string dir = m_dir.filename().string();

    fs::path dstDirPath;
    if (!context.custom.empty()) {
        // it is destintation folder fo us
        dstDirPath = fs::path(context.custom);
    } else {
        const std::string year = dir.size() >= 4 ? dir.substr(0, 4) : dir;
        dstDirPath = fs::path(m_rootSWdir) / year / dir;
    }

    const std::string dstDirPathStr = dstDirPath.string();
    if (fvaIsInternalDir(dir) || fvaIsInternalDir(dstDirPathStr)) {
        LOG_WARN << "skipped #copy for: " << m_folder.c_str() << " , dst: " << dstDirPathStr.c_str();
        return FVA_NO_ERROR;
    }

    std::error_code ec;
    for (const auto& entry : fs::directory_iterator(m_dir, fs::directory_options::skip_permission_denied, ec)) {
        if (ec) {
            LOG_CRIT << "failed to enumerate dir: " << m_folder.c_str();
            return FVA_ERROR_FAILED_TO_ENUMERATE_DIR;
        }

        std::error_code entryEc;
        if (entry.is_directory(entryEc) || entryEc) continue;

        const fs::path srcPath = entry.path();
        const fs::path existingDstPath = dstDirPath / srcPath.filename();

        std::error_code existsEc;
        if (!fs::exists(existingDstPath, existsEc)) {
            if (existsEc) {
                LOG_WARN << "failed to check destination existence: " << existingDstPath.string().c_str();
            }
            continue;
        }

        std::string croppedFileName = srcPath.stem().string();
        std::string fileNameExt = srcPath.extension().string();
        if (!fileNameExt.empty() && fileNameExt.front() == '.') fileNameExt.erase(0, 1);

        int lastDigit = -1;
        if (!croppedFileName.empty()) {
            const unsigned char lastChar = static_cast<unsigned char>(croppedFileName.back());
            if (std::isdigit(lastChar)) lastDigit = croppedFileName.back() - '0';
            if (lastDigit == 9) {
                // last digit seconds overflow
                lastDigit = -1;
            }
            croppedFileName.pop_back();
        }

        std::string newFileName = croppedFileName + std::to_string(++lastDigit);
        if (!fileNameExt.empty()) newFileName += "." + fileNameExt;

        const fs::path newFullPath = srcPath.parent_path() / newFileName;

        std::error_code newExistsEc;
        if (fs::exists(newFullPath, newExistsEc)) {
            LOG_CRIT << "destination file already exists again: " << newFullPath.string().c_str();
            return FVA_ERROR_DEST_FILE_ALREADY_EXISTS;
        }
        if (newExistsEc) {
            LOG_CRIT << "could not verify destination file: " << newFullPath.string().c_str();
            return FVA_ERROR_DEST_FILE_ALREADY_EXISTS;
        }

        std::error_code renameEc;
        fs::rename(srcPath, newFullPath, renameEc);
        if (renameEc) {
            LOG_CRIT << "could not move:" << srcPath.string().c_str() << " into " << newFullPath.string().c_str();
            return FVA_ERROR_CANT_RENAME_FILE;
        }

        LOG_DEB << "renamed:" << srcPath.string().c_str() << " into " << newFullPath.string().c_str();
        LOG_DEB << "need to fix taken time for:" << newFullPath.string().c_str();
    }
    return FVA_NO_ERROR;
}
