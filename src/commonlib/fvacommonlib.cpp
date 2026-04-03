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
#include <fstream>
#include <sstream>

#include "fvacommonexif.h"

bool fvaIsInternalFile(const std::string& fileName) { return fvaIsInternalFile(QString::fromStdString(fileName)); }
bool fvaIsFVAFile(const std::string& extention) { return FVA_FS_TYPE_UNKNOWN != fvaConvertFileExt2FileType(extention); }

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

FVA_EXIT_CODE fvaParseDirName(const std::string& dirName, QDateTime& from, QDateTime& to, const FvaFmtContext& ctx) {
    const QString qDirName = QString::fromStdString(dirName);

    switch (qDirName.length()) {
        case 4:  // one year folder
        {
            from = QDateTime::fromString(qDirName, QString::fromStdString(ctx.fvaDirNameYear));
            if (!from.isValid()) return FVA_ERROR_WRONG_FOLDER_NAME;
            to = from /*.addYears(1)*/;
        } break;
        case 9:  // year period
        {
            if (qDirName[4] != '-') return FVA_ERROR_WRONG_FOLDER_NAME;

            from = QDateTime::fromString(qDirName.mid(0, 4), QString::fromStdString(ctx.fvaDirNameYear));
            to = QDateTime::fromString(qDirName.mid(5, 4), QString::fromStdString(ctx.fvaDirNameYear));

            if (!from.isValid() || !to.isValid()) return FVA_ERROR_WRONG_FOLDER_NAME;
        } break;
        case 10:  // one-day event
        {
            from = QDateTime::fromString(qDirName, QString::fromStdString(ctx.fvaDirName));
            if (!from.isValid()) return FVA_ERROR_WRONG_FOLDER_NAME;
            to = from.addDays(1);
        } break;
        case 13: {
            from = QDateTime::fromString(qDirName.mid(0, 10), QString::fromStdString(ctx.fvaDirName));
            if (!from.isValid()) return FVA_ERROR_WRONG_FOLDER_NAME;
            if (qDirName[10] == ' ')  // one day and several events
            {
                if (qDirName[11] != '#')
                    return FVA_ERROR_WRONG_FOLDER_NAME;
                else {
                    bool result = false;
                    int dEventNumber = qDirName.mid(12, 1).toInt(&result);
                    if (!result || !dEventNumber) return FVA_ERROR_WRONG_FOLDER_NAME;
                }
                to = from.addDays(1);
            } else if (qDirName[10] == '-')  // period
            {
                QString sEndDate = qDirName.mid(11, 2);
                QString sStartDate = qDirName.mid(8, 2);
                bool res, res1 = false;
                int dEndDate = sEndDate.toInt(&res);
                int dStartDate = sStartDate.toInt(&res1);
                if (!res || !res1 || !dEndDate || !dStartDate) return FVA_ERROR_WRONG_FOLDER_NAME;
                to = from.addDays(dEndDate - dStartDate);
                to = to.addDays(1);
            } else
                return FVA_ERROR_WRONG_FOLDER_NAME;
        } break;
        case 16:  // months-day period
        {
            from = QDateTime::fromString(qDirName.mid(0, 10), QString::fromStdString(ctx.fvaDirName));
            if (!from.isValid()) return FVA_ERROR_WRONG_FOLDER_NAME;
            if (qDirName[10] != '-')  // not a period
                return FVA_ERROR_WRONG_FOLDER_NAME;

            QString sTo = qDirName.mid(0, 4) + "." + qDirName.mid(11, 5);
            to = QDateTime::fromString(sTo, QString::fromStdString(ctx.fvaDirName));
            if (!to.isValid()) return FVA_ERROR_WRONG_FOLDER_NAME;
            to = to.addDays(1);
        } break;
        default:
            return FVA_ERROR_WRONG_FOLDER_NAME;
    }
    return FVA_NO_ERROR;
}

FVA_EXIT_CODE fvaParseFileName(const std::string& fileName, QDateTime& date, const FvaFmtContext& ctx) {
    const QString qFileName = QString::fromStdString(fileName);

    if (qFileName.contains("IMG_") && qFileName.length() == 19) {
        // it is also file name to extract name from "IMG_20150504_142546"
        QString newFileName = qFileName;
        newFileName.remove("IMG_");
        date = QDateTime::fromString(newFileName, QString::fromStdString(ctx.fileName1));
        if (!date.isValid())
            return FVA_ERROR_WRONG_FILE_NAME;
        else
            return FVA_NO_ERROR;
    } else if (qFileName.contains("WP_") && qFileName.length() == 24) {
        // it is also file name to extract name from "WP_20151220_13_49_40_Pro"
        QString newFileName = qFileName;
        newFileName.remove("WP_");
        newFileName.remove("_Pro");
        date = QDateTime::fromString(newFileName, QString::fromStdString(ctx.fileName2));
        if (!date.isValid())
            return FVA_ERROR_WRONG_FILE_NAME;
        else
            return FVA_NO_ERROR;
    } else if (qFileName.contains("_") && qFileName.length() == 15) {
        // it is also file name to extract name from "20150504_142546"
        QString newFileName = qFileName;
        date = QDateTime::fromString(newFileName, QString::fromStdString(ctx.fileName1));
        if (!date.isValid())
            return FVA_ERROR_WRONG_FILE_NAME;
        else
            return FVA_NO_ERROR;
    }

    date = QDateTime::fromString(qFileName, QString::fromStdString(ctx.fvaFileName));
    if (!date.isValid()) {
        QString newFileName = QString(qFileName).replace("##", "01");
        date = QDateTime::fromString(newFileName, QString::fromStdString(ctx.fvaFileName));
        if (!date.isValid()) {
            return FVA_ERROR_WRONG_FILE_NAME;
        }
    }
    return FVA_NO_ERROR;
}

DEVICE_MAP fvaGetDeviceMapForImg(const DEVICE_MAP& deviceMap, const std::string& pathToFile, std::string& deviceName) {
    QString qtDeviceName = fvaGetExifMakeAndModelFromFile(QString::fromStdString(pathToFile));
    deviceName = qtDeviceName.toStdString();

    DEVICE_MAP result;
    if (qtDeviceName.isEmpty()) return DEVICE_MAP();
    QString fixedDevName = qtDeviceName.toUpper().trimmed();
    for (auto it = deviceMap.begin(); it != deviceMap.end(); ++it) {
        QString name = QString::fromStdString(it->second.linkedName).toUpper();
        if (name == fixedDevName) result[it->first] = it->second;
    }

    qtDeviceName = qtDeviceName.remove("  ");
    qtDeviceName = qtDeviceName.remove(QChar('\0'));
    if (!qtDeviceName.isEmpty()) {
        for (auto it = deviceMap.begin(); it != deviceMap.end(); ++it) {
            if (QString::fromStdString(it->second.linkedName) == qtDeviceName.toUpper().trimmed())
                result[it->first] = it->second;
        }
    }
    deviceName = qtDeviceName.toStdString();
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

bool fvaIsInternalDir(const std::string& dir) {
    return dir.find('#') != std::string::npos;
}
bool fvaRemoveDirIfEmpty(const std::string& dirPath) {
    const QString qDirPath = QString::fromStdString(dirPath);
    if (QDir(qDirPath).entryInfoList(QDir::NoDotAndDotDot | QDir::AllEntries).count() == 0) {
        QDir dir(qDirPath);

        // empty folder now - no need in it to keep
        return dir.rmdir(qDirPath);
    } else
        return false;
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

FVA_EXIT_CODE fvaCreateDirIfNotExists(const std::string& dirPath) {
    if (!QDir(QString::fromStdString(dirPath)).exists()) {
        QDir dir(QString::fromStdString(dirPath));

        if (!dir.mkdir(QString::fromStdString(dirPath)))
            return FVA_ERROR_CANT_CREATE_DIR;
        else
            return FVA_NO_ERROR;
    } else
        return FVA_ERROR_DEST_DIR_ALREADY_EXISTS;
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
