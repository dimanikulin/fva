/*!
 * \file fvacommonlib.cpp
 * \copyright Copyright 2021 FVA Software. All rights reserved. This file is released under the XXX License.
 * \author Dima Nikulin.
 * \version 0.29
 * \date  2014-2021
 */
#include "fvacommonlib.h"

#include <QtCore/QCoreApplication>
#include <QtCore/QDir>
#include <QtCore/QProcess>
#include <QtCore/QStringList>

#include <algorithm>
#include <cctype>
#include <fstream>
#include <sstream>

#include "fvacommonexif.h"

namespace {
std::string fvaToUpper(std::string value) {
    std::transform(value.begin(), value.end(), value.begin(), [](unsigned char ch) { return std::toupper(ch); });
    return value;
}
}  // namespace

bool fvaIsInternalFile(const QString& fileName) {
    return fileName.toUpper() == FVA_BACKGROUND_MUSIC_FILE_NAME.toUpper() || fileName.toUpper() == FVA_DB_NAME;
}

bool fvaIsInternalFile(const std::string& fileName) { return fvaIsInternalFile(QString::fromStdString(fileName)); }

bool fvaIsFVAFile(const QString& extention) { return FVA_FS_TYPE_UNKNOWN != fvaConvertFileExt2FileType(extention); }

bool fvaIsFVAFile(const std::string& extention) { return FVA_FS_TYPE_UNKNOWN != fvaConvertFileExt2FileType(extention); }

FVA_FS_TYPE fvaConvertFileExt2FileType(const QString& extention) {
    return fvaConvertFileExt2FileType(extention.toStdString());
}

FVA_FS_TYPE fvaConvertFileExt2FileType(const std::string& extention) {
    const std::string upper = fvaToUpper(extention);
    if (upper == "JPG" || upper == "JPEG"
        //|| extention.toUpper()	== "PNG"
        //|| extention.toUpper()	== "BMP"
        //|| extention.toUpper()	== "GIF"
    )
        return FVA_FS_TYPE_IMG;

    if (upper == "AVI" || upper == "MOV" || upper == "MPG" || upper == "MP4" || upper == "3GP" ||
        upper == "MKV")
        return FVA_FS_TYPE_VIDEO;

    if (upper == "WAV") return FVA_FS_TYPE_AUDIO;

    return FVA_FS_TYPE_UNKNOWN;
}

FVA_EXIT_CODE fvaParseDirName(const QString& dirName, QDateTime& from, QDateTime& to, const FvaFmtContext& ctx) {
    switch (dirName.length()) {
        case 4: {
            from = QDateTime::fromString(dirName, QString::fromStdString(ctx.fvaDirNameYear));
            if (!from.isValid()) return FVA_ERROR_WRONG_FOLDER_NAME;
            to = from;
        } break;
        case 9: {
            if (dirName[4] != '-') return FVA_ERROR_WRONG_FOLDER_NAME;

            from = QDateTime::fromString(dirName.mid(0, 4), QString::fromStdString(ctx.fvaDirNameYear));
            to = QDateTime::fromString(dirName.mid(5, 4), QString::fromStdString(ctx.fvaDirNameYear));

            if (!from.isValid() || !to.isValid()) return FVA_ERROR_WRONG_FOLDER_NAME;
        } break;
        case 10: {
            from = QDateTime::fromString(dirName, QString::fromStdString(ctx.fvaDirName));
            if (!from.isValid()) return FVA_ERROR_WRONG_FOLDER_NAME;
            to = from.addDays(1);
        } break;
        case 13: {
            from = QDateTime::fromString(dirName.mid(0, 10), QString::fromStdString(ctx.fvaDirName));
            if (!from.isValid()) return FVA_ERROR_WRONG_FOLDER_NAME;
            if (dirName[10] == ' ') {
                if (dirName[11] != '#') {
                    return FVA_ERROR_WRONG_FOLDER_NAME;
                }

                bool result = false;
                const int eventNumber = dirName.mid(12, 1).toInt(&result);
                if (!result || !eventNumber) return FVA_ERROR_WRONG_FOLDER_NAME;
                to = from.addDays(1);
            } else if (dirName[10] == '-') {
                const QString endDate = dirName.mid(11, 2);
                const QString startDate = dirName.mid(8, 2);
                bool endOk = false;
                bool startOk = false;
                const int endDay = endDate.toInt(&endOk);
                const int startDay = startDate.toInt(&startOk);
                if (!endOk || !startOk || !endDay || !startDay) return FVA_ERROR_WRONG_FOLDER_NAME;
                to = from.addDays(endDay - startDay + 1);
            } else {
                return FVA_ERROR_WRONG_FOLDER_NAME;
            }
        } break;
        case 16: {
            from = QDateTime::fromString(dirName.mid(0, 10), QString::fromStdString(ctx.fvaDirName));
            if (!from.isValid()) return FVA_ERROR_WRONG_FOLDER_NAME;
            if (dirName[10] != '-') return FVA_ERROR_WRONG_FOLDER_NAME;

            const QString toStr = dirName.mid(0, 4) + "." + dirName.mid(11, 5);
            to = QDateTime::fromString(toStr, QString::fromStdString(ctx.fvaDirName));
            if (!to.isValid()) return FVA_ERROR_WRONG_FOLDER_NAME;
            to = to.addDays(1);
        } break;
        default:
            return FVA_ERROR_WRONG_FOLDER_NAME;
    }

    return FVA_NO_ERROR;
}

FVA_EXIT_CODE fvaParseDirName(const std::string& dirName, QDateTime& from, QDateTime& to, const FvaFmtContext& ctx) {
    return fvaParseDirName(QString::fromStdString(dirName), from, to, ctx);
}

FVA_EXIT_CODE fvaParseFileName(const QString& fileName, QDateTime& date, const FvaFmtContext& ctx) {
    if (fileName.contains("IMG_") && fileName.length() == 19) {
        QString newFileName = fileName;
        newFileName.remove("IMG_");
        date = QDateTime::fromString(newFileName, QString::fromStdString(ctx.fileName1));
        return date.isValid() ? FVA_NO_ERROR : FVA_ERROR_WRONG_FILE_NAME;
    }

    if (fileName.contains("WP_") && fileName.length() == 24) {
        QString newFileName = fileName;
        newFileName.remove("WP_");
        newFileName.remove("_Pro");
        date = QDateTime::fromString(newFileName, QString::fromStdString(ctx.fileName2));
        return date.isValid() ? FVA_NO_ERROR : FVA_ERROR_WRONG_FILE_NAME;
    }

    if (fileName.contains("_") && fileName.length() == 15) {
        date = QDateTime::fromString(fileName, QString::fromStdString(ctx.fileName1));
        return date.isValid() ? FVA_NO_ERROR : FVA_ERROR_WRONG_FILE_NAME;
    }

    date = QDateTime::fromString(fileName, QString::fromStdString(ctx.fvaFileName));
    if (!date.isValid()) {
        const QString newFileName = QString(fileName).replace("##", "01");
        date = QDateTime::fromString(newFileName, QString::fromStdString(ctx.fvaFileName));
        if (!date.isValid()) return FVA_ERROR_WRONG_FILE_NAME;
    }

    return FVA_NO_ERROR;
}

FVA_EXIT_CODE fvaParseFileName(const std::string& fileName, QDateTime& date, const FvaFmtContext& ctx) {
    return fvaParseFileName(QString::fromStdString(fileName), date, ctx);
}

DEVICE_MAP fvaGetDeviceMapForImg(const DEVICE_MAP& deviceMap, const QString& pathToFile, QString& deviceName) {
    deviceName = fvaGetExifMakeAndModelFromFile(pathToFile);

    DEVICE_MAP result;
    if (deviceName.isEmpty()) return result;

    const QString fixedDevName = deviceName.toUpper().trimmed();
    for (auto it = deviceMap.begin(); it != deviceMap.end(); ++it) {
        const QString name = QString::fromStdString(it->second.linkedName).toUpper();
        if (name == fixedDevName) result[it->first] = it->second;
    }

    deviceName = deviceName.remove("  ");
    deviceName = deviceName.remove(QChar('\0'));
    if (!deviceName.isEmpty()) {
        const QString normalized = deviceName.toUpper().trimmed();
        for (auto it = deviceMap.begin(); it != deviceMap.end(); ++it) {
            if (QString::fromStdString(it->second.linkedName) == normalized) result[it->first] = it->second;
        }
    }

    return result;
}

DEVICE_MAP fvaGetDeviceMapForImg(const DEVICE_MAP& deviceMap, const std::string& pathToFile, std::string& deviceName) {
    QString qtDeviceName;
    DEVICE_MAP result = fvaGetDeviceMapForImg(deviceMap, QString::fromStdString(pathToFile), qtDeviceName);
    deviceName = qtDeviceName.toStdString();
    return result;
}

std::vector<unsigned int> fvaStringToIds(const QString& strList) {
    std::vector<unsigned int> result;
    const QStringList list = strList.split(',');
    for (const auto& item : list) result.push_back(item.toUInt());
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

bool fvaIsInternalDir(const QString& dir) { return dir.contains("#"); }

bool fvaIsInternalDir(const std::string& dir) { return dir.find('#') != std::string::npos; }

bool fvaRemoveDirIfEmpty(const QString& dirPath) {
    if (QDir(dirPath).entryInfoList(QDir::NoDotAndDotDot | QDir::AllEntries).count() == 0) {
        QDir dir(dirPath);
        return dir.rmdir(dirPath);
    }

    return false;
}

bool fvaRemoveDirIfEmpty(const std::string& dirPath) {
    return fvaRemoveDirIfEmpty(QString::fromStdString(dirPath));
}

FVA_EXIT_CODE fvaRunCLT(const std::string& cmdName, const std::string& inputDir, bool isRecursive, bool isReadOnly,
                        const std::string& custom) {
    QProcess myProcess;
    myProcess.setProcessChannelMode(QProcess::MergedChannels);
    QStringList params;
    params.append(QString::fromStdString(cmdName));
    params.append(QString::fromStdString(inputDir));
    params.append(isRecursive ? "recursive=yes" : "recursive=no");
    params.append("logvel=4");
    params.append(isReadOnly ? "readonly=yes" : "readonly=no");
    if (!custom.empty()) params.append("custom=" + QString::fromStdString(custom));

    myProcess.start("FVACLTProcess.exe", params);
    myProcess.waitForFinished(-1);

    return static_cast<FVA_EXIT_CODE>(myProcess.exitCode());
}

FVA_EXIT_CODE fvaCreateDirIfNotExists(const QString& dirPath) {
    if (!QDir(dirPath).exists()) {
        QDir dir(dirPath);
        return dir.mkdir(dirPath) ? FVA_NO_ERROR : FVA_ERROR_CANT_CREATE_DIR;
    }

    return FVA_ERROR_DEST_DIR_ALREADY_EXISTS;
}

FVA_EXIT_CODE fvaCreateDirIfNotExists(const std::string& dirPath) {
    return fvaCreateDirIfNotExists(QString::fromStdString(dirPath));
}

FVA_EXIT_CODE fvaSaveStrListToFile(const QString& path, const std::vector<QString>& strList) {
    std::vector<std::string> converted;
    converted.reserve(strList.size());
    for (const auto& item : strList) converted.push_back(item.toStdString());
    return fvaSaveStrListToFile(path.toStdString(), converted);
}

FVA_EXIT_CODE fvaSaveStrListToFile(const std::string& path, const std::vector<std::string>& strList) {
    std::ofstream fileNew(path, std::ios::app);
    if (!fileNew.is_open()) return FVA_ERROR_CANT_OPEN_FILE_DESC;

    for (const auto& entry : strList) fileNew << entry << "\n";

    if (!fileNew.good()) return FVA_ERROR_CANT_OPEN_FILE_DESC;

    return FVA_NO_ERROR;
}

FVA_EXIT_CODE fvaLoadStrListFromFile(const QString& path, std::vector<std::string>& strList) {
    return fvaLoadStrListFromFile(path.toStdString(), strList);
}

FVA_EXIT_CODE fvaLoadStrListFromFile(const std::string& path, std::vector<std::string>& strList) {
    std::ifstream file(path);
    if (!file.is_open()) return FVA_ERROR_CANT_OPEN_INPUT_FILE;

    std::string line;
    while (std::getline(file, line)) strList.push_back(line);

    if (!file.eof() && file.fail()) return FVA_ERROR_CANT_OPEN_INPUT_FILE;

    return FVA_NO_ERROR;
}
