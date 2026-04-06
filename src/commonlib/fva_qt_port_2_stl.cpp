/*!
 * \file fva_qt_port_2_stl.cpp
 * \copyright Copyright 2021 FVA Software. All rights reserved. This file is released under the XXX License.
 * \author Dima Nikulin.
 * \version 0.29
 * \date  2014-2021
 */
#include "fva_qt_port_2_stl.h"

#include <algorithm>
#include <cctype>
#include <sys/stat.h>

// TODO create UTs
std::string fvaStrToUpper(const std::string& str) {
    std::string result = str;
    std::transform(result.begin(), result.end(), result.begin(), [](unsigned char c) { return std::toupper(c); });
    return result;
}

std::string quoteArg(const std::string& arg) {
    std::string escaped;
    escaped.reserve(arg.size() + 2);
    escaped.push_back('"');
    for (char c : arg) {
        if (c == '"') escaped.push_back('\\');
        escaped.push_back(c);
    }
    escaped.push_back('"');
    return escaped;
}

bool isExistingDirectory(const std::string& dirPath) {
    struct stat pathInfo = {};
    return stat(dirPath.c_str(), &pathInfo) == 0 && ((pathInfo.st_mode & S_IFDIR) != 0);
}
