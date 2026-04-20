/*!
 * \file CLTPrintFSStructure.cpp
 * \copyright Copyright 2021 FVA Software. All rights reserved. This file is released under the XXX License.
 * \author Dima Nikulin.
 * \version 0.29
 * \date  2014-2021
 */
#include "CLTPrintFSStructure.h"

#include <algorithm>
#include <filesystem>
#include <vector>

#include <QtCore/QCryptographicHash>
#include <QtCore/QFileInfo>

CLTPrintFSStructure::CLTPrintFSStructure(const FvaConfiguration& cfg) {
    std::string rootSWdir;
    FVA_EXIT_CODE res = cfg.getParamAsString("Common::RootDir", rootSWdir);
    RET_IF_RES_IS_ERROR

    m_file.setFileName(QString::fromStdString(rootSWdir) + "#logs#/fsoutput.txt");
    m_file.open(QIODevice::WriteOnly);

    res = m_fmtctx.fillFmtContextFromCfg(cfg);
    RET_IF_RES_IS_ERROR
}

CLTPrintFSStructure::~CLTPrintFSStructure() { m_file.close(); }

FVA_EXIT_CODE CLTPrintFSStructure::execute(const CLTContext& /*context*/) {
    namespace fs = std::filesystem;

    char buffer[64 * 1024];
    qint64 size = 0;
    QString result;

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

        const QString absoluteFilePath = QString::fromStdString(entry.path().string());
        QFile file(absoluteFilePath);

        if (!file.open(QIODevice::ReadOnly)) {
            LOG_CRIT << "failed to open file:" << absoluteFilePath;
            continue;
        }

        QCryptographicHash hash(QCryptographicHash::Sha1);
        while (!file.atEnd()) {
            size = file.read(buffer, 64 * 1024);
            if (size) hash.addData(buffer, size);
        }

        QFileInfo info(absoluteFilePath);
        result = absoluteFilePath + "," + hash.result().toBase64() + "," +
                 info.lastModified().toString(QString::fromStdString(m_fmtctx.fvaFileName)) + "," +
                 QString::number(info.size()) + "\n";
        m_file.write(result.toLocal8Bit());
    }
    return FVA_NO_ERROR;
}
