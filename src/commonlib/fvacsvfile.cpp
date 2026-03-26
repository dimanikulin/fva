/*!
 * \file fvacsvfile.cpp
 * \copyright Copyright 2021 FVA Software. All rights reserved. This file is released under the XXX License.
 * \author Dima Nikulin.
 * \version 0.29
 * \date  2014-2021
 */

#include "fvacsvfile.h"

#include <ios>

CSVFile::CSVFile() {}

CSVFile::~CSVFile() {
    if (m_outputFile.get()) {
        m_outputFile->flush();
        m_outputFile->close();
        m_outputFile.release();
    }

    if (m_inputFile.get()) {
        m_inputFile->close();
        m_inputFile.release();
    }
}

bool CSVFile::openForRead(const std::string& path) {
    m_inputFile.reset(new std::ifstream(path.c_str(), std::ios::in));
    return m_inputFile->is_open();
}

bool CSVFile::openForWrite(const std::string& path) {
    m_outputFile.reset(new std::ofstream(path.c_str(), std::ios::out | std::ios::trunc));
    return m_outputFile->is_open();
}

std::string CSVFile::readLine() {
    std::string line;
    if (!m_inputFile.get() || !std::getline(*m_inputFile, line)) return std::string();

    if (!line.empty() && line[line.size() - 1] == '\r') {
        line.erase(line.size() - 1);
    }
    return line;
}

bool CSVFile::atEnd() { return !m_inputFile.get() || m_inputFile->eof(); }

bool CSVFile::parseLine(const std::string& line, std::vector<std::string>& values, char delimiter) {
    std::size_t pos = 0;
    bool quotes = false;
    std::string field;

    while (pos < line.length() && line[pos] != 0x00) {
        const char c = line[pos];
        if (!quotes && c == '"')
            quotes = true;
        else if (quotes && c == '"') {
            if (pos + 1 < line.length() && line[pos + 1] == '"') {
                field.push_back(c);
                pos++;
            } else
                quotes = false;
        } else if (!quotes && c == delimiter) {
            values.push_back(field);
            field.clear();
        } else if (!quotes && (c == 0x0A || c == 0x0D)) {
            values.push_back(field);
            field.clear();
        } else
            field.push_back(c);
        pos++;
        if (pos >= line.size())  // it was last symbol
            values.push_back(field);
    }

    return true;
}

bool CSVFile::writeLine(const std::string& line) {
    if (!m_outputFile.get()) return false;

    *m_outputFile << line << "\n";
    return static_cast<bool>(*m_outputFile);
}
