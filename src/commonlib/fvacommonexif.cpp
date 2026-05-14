/*!
 * \file fvacommonexif.cpp
 * \copyright Copyright 2021 FVA Software. All rights reserved. This file is released under the XXX License.
 * \author Dima Nikulin.
 * \version 0.29
 * \date  2014-2021
 */

#include "fvacommonexif.h"

#include <chrono>
#include <fstream>
#include <iomanip>
#include <sstream>
#include <vector>

#include "exif.h"
#include "fvariffparser.h"

std::string fvaGetExifMakeAndModelFromFile(const std::string& pathToFile) {
    std::ifstream file(pathToFile, std::ios::binary);
    if (!file.is_open()) return "";
    std::vector<unsigned char> data((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    easyexif::EXIFInfo info;
    if (0 == info.parseFrom(data.data(), static_cast<unsigned int>(data.size()))) {
        return info.Make + info.Model;
    }
    return "";
}

std::chrono::system_clock::time_point fvaGetExifDateTimeOriginalFromFile(const std::string& pathToFile,
                                                                         const std::string& exifDateTimeFmt) {
    std::ifstream file(pathToFile, std::ios::binary);
    if (!file.is_open()) return {};
    std::vector<unsigned char> data((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    easyexif::EXIFInfo info;
    if (0 == info.parseFrom(data.data(), static_cast<unsigned int>(data.size()))) {
        std::tm tm = {};
        std::istringstream ss(info.DateTimeOriginal);
        ss >> std::get_time(&tm, exifDateTimeFmt.c_str());
        if (!ss.fail()) {
            return std::chrono::system_clock::from_time_t(std::mktime(&tm));
        }
    }
    return {};
}
bool fvaExifGeoDataPresentInFile(const std::string& pathToFile) {
    std::ifstream file(pathToFile, std::ios::binary);
    if (!file.is_open()) return false;
    std::vector<unsigned char> data((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    easyexif::EXIFInfo info;
    if (0 == info.parseFrom(data.data(), static_cast<unsigned int>(data.size()))) {
        easyexif::EXIFInfo::Geolocation_t loc = info.GeoLocation;
        return loc.Latitude > 0 && loc.Longitude > 0;
    }
    return false;
}
std::chrono::system_clock::time_point fvaGetVideoTakenTime(const std::string& pathToFile, std::string& error,
                                                           const FvaFmtContext& ctx) {
    std::chrono::system_clock::time_point renameDateTime =
        fvaGetExifDateTimeOriginalFromFile(pathToFile, ctx.exifDateTime);
    if (renameDateTime == std::chrono::system_clock::time_point{}) {
        RiffParser riffInfo;
        std::string createdDate;
        if (!riffInfo.open(pathToFile, error) || !riffInfo.findTag("IDIT", createdDate) ||
            !riffInfo.convertToDate(createdDate, renameDateTime, ctx))
            return renameDateTime;
    }
    return renameDateTime;
}
