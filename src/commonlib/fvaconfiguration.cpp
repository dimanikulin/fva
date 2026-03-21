/*!
 * \file fvaconfiguration.cpp
 * \copyright Copyright 2021 FVA Software. All rights reserved. This file is released under the XXX License.
 * \author Dima Nikulin.
 * \version 0.29
 * \date  2014-2021
 */

#include "fvaconfiguration.h"

#include <QtCore/QString>
#include <QtCore/QStringList>
#include <algorithm>
#include <cctype>
#include <cstdlib>

namespace {
// TODO : to move this function to some common place and use in other places as well
// TODO make UT for this function
std::string toUpperCopy(const std::string& value) {
    std::string result = value;
    std::transform(result.begin(), result.end(), result.begin(),
                   [](unsigned char ch) { return static_cast<char>(std::toupper(ch)); });
    return result;
}

int getColumnIdByName(const std::vector<std::string>& titles, const std::string& columnName) {
    for (size_t i = 0; i < titles.size(); ++i) {
        if (titles[i] == columnName) {
            return static_cast<int>(i);
        }
    }
    return -1;
}
}  // namespace

FVA_EXIT_CODE FvaConfiguration::load(const std::string& path) {
    FVADescriptionFile fvaFileCsv;
    QStringList cfgTitlesQt;
    DESCRIPTIONS_MAP cfgItemsQt;

    FVA_EXIT_CODE res = fvaFileCsv.load(QString::fromStdString(path), cfgTitlesQt, cfgItemsQt);
    RET_RES_IF_RES_IS_ERROR

    m_cfgtitles.clear();
    m_cfgtitles.reserve(static_cast<size_t>(cfgTitlesQt.size()));
    for (const QString& title : cfgTitlesQt) {
        m_cfgtitles.push_back(title.toStdString());
    }

    m_cfgItems.clear();
    for (DESCRIPTIONS_MAP::const_iterator it = cfgItemsQt.begin(); it != cfgItemsQt.end(); ++it) {
        const QStringList& row = it.value();
        std::vector<std::string> convertedRow;
        convertedRow.reserve(static_cast<size_t>(row.size()));
        for (const QString& item : row) {
            convertedRow.push_back(item.toStdString());
        }
        m_cfgItems[it.key()] = convertedRow;
    }

    // ID,Name,Value
    int columnName = getColumnIdByName(m_cfgtitles, "Name");
    if (-1 == columnName) return FVA_ERROR_CANT_FIND_MANDATORY_FIELDS;

    int columnValue = getColumnIdByName(m_cfgtitles, "Value");
    if (-1 == columnValue) return FVA_ERROR_CANT_FIND_MANDATORY_FIELDS;

    return FVA_NO_ERROR;
}
FVA_EXIT_CODE FvaConfiguration::save(const std::string& path) {
    FVADescriptionFile fvaFileCsv;

    QStringList cfgTitlesQt;
    for (const std::string& title : m_cfgtitles) {
        cfgTitlesQt.append(QString::fromStdString(title));
    }

    DESCRIPTIONS_MAP cfgItemsQt;
    for (std::map<int, std::vector<std::string>>::const_iterator it = m_cfgItems.begin(); it != m_cfgItems.end();
         ++it) {
        QStringList rowQt;
        for (const std::string& item : it->second) {
            rowQt.append(QString::fromStdString(item));
        }
        cfgItemsQt[it->first] = rowQt;
    }

    return fvaFileCsv.save(QString::fromStdString(path), cfgTitlesQt, cfgItemsQt);
}

FVA_EXIT_CODE FvaConfiguration::getParamAsString(const std::string& paramName, std::string& paramValue) const {
    // ID,Name,Value
    int columnName = getColumnIdByName(m_cfgtitles, "Name");
    if (-1 == columnName) return FVA_ERROR_CANT_FIND_MANDATORY_FIELDS;

    int columnValue = getColumnIdByName(m_cfgtitles, "Value");
    if (-1 == columnValue) return FVA_ERROR_CANT_FIND_MANDATORY_FIELDS;

    const std::string upperParamName = toUpperCopy(paramName);
    for (std::map<int, std::vector<std::string>>::const_iterator it = m_cfgItems.begin(); it != m_cfgItems.end();
         ++it) {
        const std::vector<std::string>& list = it->second;
        if (columnName >= static_cast<int>(list.size()) || columnValue >= static_cast<int>(list.size())) {
            continue;
        }
        const std::string upperParamNameFromCfg = toUpperCopy(list[static_cast<size_t>(columnName)]);
        if (upperParamName == upperParamNameFromCfg) {
            paramValue = list[static_cast<size_t>(columnValue)];
            return FVA_NO_ERROR;
        }
    }
    return FVA_ERROR_CANT_GET_PARAM;
}

FVA_EXIT_CODE FvaConfiguration::getParamAsBoolean(const std::string& paramName, bool& paramValue) const {
    std::string str;
    FVA_EXIT_CODE res = getParamAsString(paramName, str);
    RET_RES_IF_RES_IS_ERROR
    const std::string upper = toUpperCopy(str);
    if (upper == "FALSE" || upper == "NO") {
        paramValue = false;
        return FVA_NO_ERROR;
    }
    if (upper == "TRUE" || upper == "YES") {
        paramValue = true;
        return FVA_NO_ERROR;
    }
    return FVA_ERROR_CANT_GET_PARAM;
}

FVA_EXIT_CODE FvaConfiguration::getParamAsUint(const std::string& paramName, unsigned int& paramValue) const {
    std::string str;
    FVA_EXIT_CODE res = getParamAsString(paramName, str);
    RET_RES_IF_RES_IS_ERROR
    paramValue = static_cast<unsigned int>(std::strtoul(str.c_str(), nullptr, 10));
    return FVA_NO_ERROR;
}

FVA_EXIT_CODE FvaConfiguration::setParam(const std::string& paramName, const std::string& paramValue) {
    // ID,Name,Value
    int columnName = getColumnIdByName(m_cfgtitles, "Name");
    if (-1 == columnName) return FVA_ERROR_CANT_FIND_MANDATORY_FIELDS;

    int columnValue = getColumnIdByName(m_cfgtitles, "Value");
    if (-1 == columnValue) return FVA_ERROR_CANT_FIND_MANDATORY_FIELDS;

    const std::string upperParamName = toUpperCopy(paramName);
    for (std::map<int, std::vector<std::string>>::iterator it = m_cfgItems.begin(); it != m_cfgItems.end(); ++it) {
        std::vector<std::string>& list = it->second;
        if (columnName >= static_cast<int>(list.size()) || columnValue >= static_cast<int>(list.size())) {
            continue;
        }
        const std::string upperParamNameFromCfg = toUpperCopy(list[static_cast<size_t>(columnName)]);
        if (upperParamName == upperParamNameFromCfg) {
            list[static_cast<size_t>(columnValue)] = paramValue;
            return FVA_NO_ERROR;
        }
    }
    return FVA_ERROR_CANT_SET_PARAM;
}

FVA_EXIT_CODE FvaConfiguration::setParam(const std::string& paramName, bool paramValue) {
    return setParam(paramName, std::string(paramValue ? "TRUE" : "FALSE"));
}

FVA_EXIT_CODE FvaConfiguration::setParam(const std::string& paramName, unsigned int paramValue) {
    return setParam(paramName, std::to_string(paramValue));
}
