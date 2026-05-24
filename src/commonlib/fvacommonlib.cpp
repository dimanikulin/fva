/*!
 * \file fvacommonlib.cpp
 * \copyright Copyright 2021 FVA Software. All rights reserved. This file is released under the XXX License.
 * \author Dima Nikulin.
 * \version 0.29
 * \date  2014-2021
 */
#include "fvacommonlib.h"

#include <algorithm>
#include <cctype>
#include <cstdlib>
#include <ctime>
#include <filesystem>
#include <fstream>
#include <iomanip>
#include <sstream>

#include "fva_qt_port_2_stl.h"
#include "fvacommonexif.h"

bool fvaIsInternalFile(const std::string& fileName) {
    const std::string upperName = fvaStrToUpper(fileName);
    return upperName == fvaStrToUpper(FVA_BACKGROUND_MUSIC_FILE_NAME) || upperName == fvaStrToUpper(FVA_DB_NAME);
}

bool fvaIsFVAFile(const std::string& extention) { return FVA_FS_TYPE_UNKNOWN != fvaConvertFileExt2FileType(extention); }

FVA_FS_TYPE fvaConvertFileExt2FileType(const std::string& extention) {
    const std::string upper = fvaStrToUpper(extention);
    if (upper == "JPG" || upper == "JPEG"
        //|| extention.toUpper()	== "PNG"
        //|| extention.toUpper()	== "BMP"
        //|| extention.toUpper()	== "GIF"
    )
        return FVA_FS_TYPE_IMG;

    if (upper == "AVI" || upper == "MOV" || upper == "MPG" || upper == "MP4" || upper == "3GP" || upper == "MKV")
        return FVA_FS_TYPE_VIDEO;

    if (upper == "WAV") return FVA_FS_TYPE_AUDIO;

    return FVA_FS_TYPE_UNKNOWN;
}

FVA_EXIT_CODE fvaParseDirName(const std::string& dirName, std::tm& from, std::tm& to, const FvaFmtContext& ctx) {
    switch (dirName.size()) {
        case 4: {
            if (!parseDateTime(dirName, ctx.fvaDirNameYear, from)) return FVA_ERROR_WRONG_FOLDER_NAME;
            to = from;
        } break;
        case 9: {
            if (dirName[4] != '-') return FVA_ERROR_WRONG_FOLDER_NAME;
            if (!parseDateTime(dirName.substr(0, 4), ctx.fvaDirNameYear, from) ||
                !parseDateTime(dirName.substr(5, 4), ctx.fvaDirNameYear, to)) {
                return FVA_ERROR_WRONG_FOLDER_NAME;
            }
        } break;
        case 10: {
            if (!parseDateTime(dirName, ctx.fvaDirName, from)) return FVA_ERROR_WRONG_FOLDER_NAME;
            to = from;
            if (!addDays(to, 1)) return FVA_ERROR_WRONG_FOLDER_NAME;
        } break;
        case 13: {
            if (!parseDateTime(dirName.substr(0, 10), ctx.fvaDirName, from)) return FVA_ERROR_WRONG_FOLDER_NAME;
            if (dirName[10] == ' ') {
                if (dirName[11] != '#') return FVA_ERROR_WRONG_FOLDER_NAME;
                if (dirName[12] < '1' || dirName[12] > '9') return FVA_ERROR_WRONG_FOLDER_NAME;
                to = from;
                if (!addDays(to, 1)) return FVA_ERROR_WRONG_FOLDER_NAME;
            } else if (dirName[10] == '-') {
                int startDay = 0;
                int endDay = 0;
                try {
                    startDay = std::stoi(dirName.substr(8, 2));
                    endDay = std::stoi(dirName.substr(11, 2));
                } catch (...) {
                    return FVA_ERROR_WRONG_FOLDER_NAME;
                }
                if (startDay <= 0 || endDay <= 0) return FVA_ERROR_WRONG_FOLDER_NAME;
                to = from;
                if (!addDays(to, endDay - startDay + 1)) return FVA_ERROR_WRONG_FOLDER_NAME;
            } else {
                return FVA_ERROR_WRONG_FOLDER_NAME;
            }
        } break;
        case 16: {
            if (!parseDateTime(dirName.substr(0, 10), ctx.fvaDirName, from)) return FVA_ERROR_WRONG_FOLDER_NAME;
            if (dirName[10] != '-') return FVA_ERROR_WRONG_FOLDER_NAME;

            const std::string toDate = dirName.substr(0, 4) + "." + dirName.substr(11, 5);
            if (!parseDateTime(toDate, ctx.fvaDirName, to)) return FVA_ERROR_WRONG_FOLDER_NAME;
            if (!addDays(to, 1)) return FVA_ERROR_WRONG_FOLDER_NAME;
        } break;
        default:
            return FVA_ERROR_WRONG_FOLDER_NAME;
    }

    return FVA_NO_ERROR;
}

FVA_EXIT_CODE fvaParseFileName(const std::string& fileName, std::tm& date, const FvaFmtContext& ctx) {
    if (fileName.find("IMG_") != std::string::npos && fileName.size() == 19) {
        if (!parseDateTime(fileName.substr(4), ctx.fileName1, date)) return FVA_ERROR_WRONG_FILE_NAME;
        return FVA_NO_ERROR;
    }

    if (fileName.find("WP_") != std::string::npos && fileName.size() == 24) {
        const std::string core = fileName.substr(3, fileName.size() - 7);
        if (!parseDateTime(core, ctx.fileName2, date)) return FVA_ERROR_WRONG_FILE_NAME;
        return FVA_NO_ERROR;
    }

    if (fileName.find('_') != std::string::npos && fileName.size() == 15) {
        if (!parseDateTime(fileName, ctx.fileName1, date)) return FVA_ERROR_WRONG_FILE_NAME;
        return FVA_NO_ERROR;
    }

    if (parseDateTime(fileName, ctx.fvaFileName, date)) return FVA_NO_ERROR;

    std::string updatedName = fileName;
    std::size_t pos = 0;
    while ((pos = updatedName.find("##", pos)) != std::string::npos) {
        updatedName.replace(pos, 2, "01");
        pos += 2;
    }

    if (!parseDateTime(updatedName, ctx.fvaFileName, date)) return FVA_ERROR_WRONG_FILE_NAME;
    return FVA_NO_ERROR;
}

DEVICE_MAP fvaGetDeviceMapForImg(const DEVICE_MAP& deviceMap, const std::string& pathToFile, std::string& deviceName) {
    std::string devName = fvaGetExifMakeAndModelFromFile(pathToFile);
    deviceName = devName;

    DEVICE_MAP result;
    if (devName.empty()) return DEVICE_MAP();
    std::string fixedDevName = fvaStrToUpper(trim(devName));
    for (auto it = deviceMap.begin(); it != deviceMap.end(); ++it) {
        std::string name = fvaStrToUpper(it->second.linkedName);
        if (name == fixedDevName) result[it->first] = it->second;
    }

    // remove double spaces and null characters
    std::string cleaned;
    for (char c : devName) {
        if (c != '\0') cleaned += c;
    }
    while (cleaned.find("  ") != std::string::npos) cleaned.replace(cleaned.find("  "), 2, " ");
    if (!cleaned.empty()) {
        std::string cleanedUpper = fvaStrToUpper(trim(cleaned));
        for (auto it = deviceMap.begin(); it != deviceMap.end(); ++it) {
            if (fvaStrToUpper(it->second.linkedName) == cleanedUpper) result[it->first] = it->second;
        }
    }
    deviceName = cleaned;
    return result;
}

std::vector<unsigned int> fvaStringToIds(const std::string& strList) {
    std::vector<unsigned int> result;
    std::stringstream stream(strList);
    std::string item;

    while (std::getline(stream, item, ',')) {
        if (!item.empty()) {
            result.push_back(static_cast<unsigned int>(std::stoul(item)));
        }
    }

    return result;
}

bool fvaIsInternalDir(const std::string& dir) { return dir.find('#') != std::string::npos; }
bool fvaRemoveDirIfEmpty(const std::string& dirPath) {
    namespace fs = std::filesystem;
    std::error_code ec;
    const fs::path path(dirPath);

    if (!fs::exists(path, ec) || ec || !fs::is_directory(path, ec) || ec) return false;
    if (!fs::is_empty(path, ec) || ec) return false;

    // empty folder now - no need to keep it
    return fs::remove(path, ec) && !ec;
}

FVA_EXIT_CODE fvaRunCLT(const std::string& cmdName, const std::string& inputDir, bool isRecursive, bool isReadOnly,
                        const std::string& custom) {
    std::string command = quoteArg("FVACLTProcess.exe") + " " + quoteArg(cmdName) + " " + quoteArg(inputDir) + " " +
                          quoteArg(isRecursive ? "recursive=yes" : "recursive=no") + " " + quoteArg("logvel=4") + " " +
                          quoteArg(isReadOnly ? "readonly=yes" : "readonly=no");
    if (!custom.empty()) command += " " + quoteArg("custom=" + custom);

    const int result = std::system(command.c_str());
    if (result == -1) return FVA_ERROR_CANT_START_PYTHON_PROC;
    return static_cast<FVA_EXIT_CODE>(result);
}

FVA_EXIT_CODE fvaCreateDirIfNotExists(const std::string& dirPath) {
    namespace fs = std::filesystem;
    std::error_code ec;
    const fs::path path(dirPath);

    if (fs::exists(path, ec)) return FVA_ERROR_DEST_DIR_ALREADY_EXISTS;
    if (ec) return FVA_ERROR_CANT_CREATE_DIR;

    if (!fs::create_directories(path, ec) || ec) return FVA_ERROR_CANT_CREATE_DIR;
    return FVA_NO_ERROR;
}

FVA_EXIT_CODE fvaSaveStrListToFile(const std::string& path, const std::vector<std::string>& strList) {
    std::ofstream fileNew(path, std::ios::app);
    if (!fileNew.is_open()) return FVA_ERROR_CANT_OPEN_FILE_DESC;

    for (const auto& entry : strList) fileNew << entry << "\n";

    if (!fileNew.good()) return FVA_ERROR_CANT_OPEN_FILE_DESC;

    return FVA_NO_ERROR;
}

FVA_EXIT_CODE fvaLoadStrListFromFile(const std::string& path, std::vector<std::string>& strList) {
    std::ifstream file(path);
    if (!file.is_open()) return FVA_ERROR_CANT_OPEN_INPUT_FILE;

    std::string line;
    while (std::getline(file, line)) strList.push_back(line);

    if (!file.eof() && file.fail()) return FVA_ERROR_CANT_OPEN_INPUT_FILE;

    return FVA_NO_ERROR;
}
