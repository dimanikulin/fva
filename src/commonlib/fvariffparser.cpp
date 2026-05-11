/*!
 * \file fvariffparser.cpp
 * \copyright Copyright 2021 FVA Software. All rights reserved. This file is released under the XXX License.
 * \author Dima Nikulin.
 * \version 0.29
 * \date  2014-2021
 */

#include "fvariffparser.h"
#include <cstring>
#include <iomanip>
#include <sstream>

// Helper function to parse date string
std::chrono::system_clock::time_point parseDateTime(const std::string& strDate, const std::string& format) {
    std::tm tm = {};
    std::istringstream ss(strDate);
    ss >> std::get_time(&tm, format.c_str());
    if (ss.fail()) {
        return std::chrono::system_clock::from_time_t(0);
    }
    auto tp = std::chrono::system_clock::from_time_t(std::mktime(&tm));
    return tp;
}

bool RiffParser::open(const std::string& path, std::string& error) {
    m_file.reset(new std::ifstream(path, std::ios::binary));
    if (!m_file->is_open()) {
        error = "Can not open file";
        return false;
    }

    char buffer[12];

    // check format
    m_file->read(buffer, sizeof(buffer));
    std::string fourcc(buffer, 4);
    if (fourcc != "RIFF" && fourcc != "RIFX") {
        error = "Incorrect FourCC";
        return false;
    }

    int datasize = *((int*)(&buffer[4])) + sizeof(int64_t);
    // check size
    m_file->seekg(0, std::ios::end);
    std::streamsize filesize = m_file->tellg();
    m_file->seekg(0, std::ios::beg);
    if (filesize < datasize) {
        error = "Incorrect size";
        return false;
    }
    return true;
}
bool RiffParser::processNode(const std::string& tag, std::string& value) {
    while (m_file->good()) {
        // Examine the element, is it a list or a chunk
        char buffer[8];
        m_file->read(buffer, sizeof(buffer));
        if (m_file->gcount() < sizeof(buffer)) break;
        
        int datasize = *((int*)(&buffer[4]));
        std::string fourcc(buffer, 4);
        
        if (fourcc == "LIST") {
            char buffer_[4];
            m_file->read(buffer_, sizeof(buffer_));
            if (m_file->gcount() < sizeof(buffer_)) break;
            if (processNode(tag, value)) return true;  // found wanted element
        } else {
            int paddedSize = datasize;
            if (0 != (datasize & 1)) ++paddedSize;
            if (tag == fourcc) {
                char buffer_[1024];
                m_file->read(buffer_, std::min((int)sizeof(buffer_), datasize));
                value = std::string(buffer_, datasize);
                return true;
            }
            m_file->seekg(paddedSize, std::ios::cur);
        }
    }
    return false;
}
bool RiffParser::findTag(const std::string& tag, std::string& value) { 
    m_file->seekg(0, std::ios::beg);
    return processNode(tag, value);
}
bool RiffParser::convertToDate(const std::string& strDate, std::chrono::system_clock::time_point& value, const FvaFmtContext& ctx) const {
    // remove terminate char and last symbol
    std::string trimmed = strDate.substr(4, strDate.size() - 5);
    trimmed.erase(trimmed.find_last_not_of("\n") + 1);
    
    value = parseDateTime(trimmed, ctx.riffDateTime1);
    if (value.time_since_epoch().count() != 0) {
        return true;
    }
    
    // try to use other format
    std::string newStr = strDate.substr(0, strDate.size() - 1);
    newStr.erase(newStr.find_last_not_of("\n") + 1);
    value = parseDateTime(newStr, ctx.exifDateTime);
    if (value.time_since_epoch().count() != 0) {
        return true;
    }

    // try to use other format
    std::string utcPrefix = "UTC ";
    std::string newStr2 = strDate.substr(utcPrefix.size());
    newStr2.erase(newStr2.find_last_not_of("\n") + 1);
    value = parseDateTime(newStr2, ctx.riffDateTime2);

    return value.time_since_epoch().count() != 0;
}
