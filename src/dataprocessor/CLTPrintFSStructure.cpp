/*!
 * \file CLTPrintFSStructure.cpp
 * \copyright Copyright 2021 FVA Software. All rights reserved. This file is released under the XXX License.
 * \author Dima Nikulin.
 * \version 0.29
 * \date  2014-2021
 */
#include "CLTPrintFSStructure.h"

#include <algorithm>
#include <array>
#include <chrono>
#include <cstdint>
#include <cstring>
#include <filesystem>
#include <iomanip>
#include <sstream>
#include <vector>

#include "fva_qt_port_2_stl.h"

CLTPrintFSStructure::CLTPrintFSStructure(const FvaConfiguration& cfg) {
    std::string rootSWdir;
    FVA_EXIT_CODE res = cfg.getParamAsString("Common::RootDir", rootSWdir);
    RET_IF_RES_IS_ERROR

    const std::filesystem::path outputPath = std::filesystem::path(rootSWdir) / "#logs#" / "fsoutput.txt";
    m_file.open(outputPath, std::ios::out | std::ios::trunc);
    if (!m_file.is_open()) {
        LOG_CRIT << "failed to open output file: " << outputPath.string().c_str();
    }

    res = m_fmtctx.fillFmtContextFromCfg(cfg);
    RET_IF_RES_IS_ERROR
}

CLTPrintFSStructure::~CLTPrintFSStructure() { m_file.close(); }

FVA_EXIT_CODE CLTPrintFSStructure::execute(const CLTContext& /*context*/) {
    namespace fs = std::filesystem;

    std::array<char, 64 * 1024> buffer = {};

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

        std::ifstream file(entry.path(), std::ios::binary);

        if (!file.is_open()) {
            LOG_CRIT << "failed to open file:" << entry.path().string().c_str();
            continue;
        }

        Sha1 sha1;
        while (file.good()) {
            file.read(buffer.data(), static_cast<std::streamsize>(buffer.size()));
            const std::streamsize bytesRead = file.gcount();
            if (bytesRead > 0) {
                sha1.update(reinterpret_cast<const std::uint8_t*>(buffer.data()), static_cast<std::size_t>(bytesRead));
            }
        }

        const std::array<std::uint8_t, 20> digest = sha1.final();

        const fs::path absolutePath = fs::absolute(entry.path(), entryEc);
        if (entryEc) {
            LOG_WARN << "failed to resolve absolute path: " << entry.path().string().c_str();
            continue;
        }

        const std::uintmax_t fileSize = fs::file_size(entry.path(), entryEc);
        if (entryEc) {
            LOG_WARN << "failed to get file size: " << entry.path().string().c_str();
            continue;
        }

        const fs::file_time_type writeTime = fs::last_write_time(entry.path(), entryEc);
        if (entryEc) {
            LOG_WARN << "failed to get file write time: " << entry.path().string().c_str();
            continue;
        }

        if (!m_file.is_open()) {
            LOG_CRIT << "output file is not opened";
            return FVA_ERROR_CANT_OPEN_FILE;
        }

        m_file << absolutePath.string() << "," << base64Encode(digest.data(), digest.size()) << ","
               << formatFileTime(writeTime, m_fmtctx.fvaFileName) << "," << fileSize << "\n";
    }
    return FVA_NO_ERROR;
}
