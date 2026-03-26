/*!
 * \file FVADescriptionFile.cpp
 * \copyright Copyright 2021 FVA Software. All rights reserved. This file is released under the XXX License.
 * \author Dima Nikulin.
 * \version 0.29
 * \date  2014-2021
 */

#include "FVADescriptionFile.h"

#include <algorithm>
#include <cctype>
#include <cstdlib>

// TODO : to move these functions to some common place and use in other places as well
// TODO make UT for these functions
namespace {
std::string toUpperCopy(const std::string& value) {
    std::string result = value;
    std::transform(result.begin(), result.end(), result.begin(),
                   [](unsigned char ch) { return static_cast<char>(std::toupper(ch)); });
    return result;
}

bool isBlankLine(const std::string& line) {
    return std::find_if(line.begin(), line.end(), [](unsigned char ch) { return !std::isspace(ch); }) == line.end();
}

std::string trimCopy(const std::string& value) {
    std::string::const_iterator begin = value.begin();
    while (begin != value.end() && std::isspace(static_cast<unsigned char>(*begin))) {
        ++begin;
    }

    std::string::const_iterator end = value.end();
    while (end != begin && std::isspace(static_cast<unsigned char>(*(end - 1)))) {
        --end;
    }

    return std::string(begin, end);
}

std::string escapeField(const std::string& value) {
    std::string cleaned = trimCopy(value);
    cleaned.erase(std::remove(cleaned.begin(), cleaned.end(), '\t'), cleaned.end());

    if (cleaned.find_first_of(",\"") == std::string::npos) {
        return cleaned;
    }

    std::string escaped;
    escaped.reserve(cleaned.size());
    for (std::string::const_iterator it = cleaned.begin(); it != cleaned.end(); ++it) {
        if (*it == '"') {
            escaped.push_back('"');
        }
        escaped.push_back(*it);
    }
    return std::string("\"") + escaped + "\"";
}
}  // namespace

FVADescriptionFile::FVADescriptionFile() {}
FVADescriptionFile::~FVADescriptionFile() {}
FVA_EXIT_CODE FVADescriptionFile::load(const std::string& path, std::vector<std::string>& titles,
                                       DESCRIPTIONS_MAP& decsItems) {
    if (!openForRead(path)) return FVA_ERROR_CANT_OPEN_FILE_DESC;

    int indexOfIDColumn = -1;
    while (!atEnd()) {
        std::string line = readLine();
        if (line.empty() && atEnd()) break;
        if (isBlankLine(line)) continue;
        std::vector<std::string> values;
        if (!parseLine(line, values))
            continue;
        else {
            if (titles.empty()) {
                indexOfIDColumn = getColumnIdByName(values, "ID");
                if (-1 == indexOfIDColumn) return FVA_ERROR_CANT_FIND_MANDATORY_FIELDS;
                titles = values;
                continue;
            } else {
                if (values.size() != titles.size()) return FVA_ERROR_INCORRECT_FORMAT;
                decsItems[std::atoi(values[static_cast<std::size_t>(indexOfIDColumn)].c_str())] = values;
            }
        }
    }
    return FVA_NO_ERROR;
}
int FVADescriptionFile::getColumnIdByName(const std::vector<std::string>& titles, const std::string& columnName) {
    const std::string upperColumnName = toUpperCopy(columnName);
    for (std::size_t id = 0; id < titles.size(); ++id) {
        if (upperColumnName == toUpperCopy(titles[id])) return static_cast<int>(id);
    }
    return -1;
}
FVA_EXIT_CODE FVADescriptionFile::save(const std::string& path, const std::vector<std::string>& titles,
                                       const DESCRIPTIONS_MAP& decsItems) {
    if (!openForWrite(path)) return FVA_ERROR_CANT_OPEN_FILE_DESC;
    for (auto i = decsItems.begin(); i != decsItems.end(); ++i)
        if (i->second.size() != titles.size()) return FVA_ERROR_INCORRECT_FORMAT;

    std::string oneStr;
    for (std::size_t titleId = 0; titleId < titles.size(); ++titleId) {
        if (!oneStr.empty()) oneStr += ",";
        oneStr += escapeField(titles[titleId]);
    }
    writeLine(oneStr);
    for (auto i = decsItems.begin(); i != decsItems.end(); ++i) {
        oneStr.clear();
        for (std::vector<std::string>::const_iterator iv = i->second.begin(); iv != i->second.end(); ++iv) {
            if (!oneStr.empty()) oneStr += ",";
            oneStr += escapeField(*iv);
        }

        writeLine(oneStr);
    }
    return FVA_NO_ERROR;
}
