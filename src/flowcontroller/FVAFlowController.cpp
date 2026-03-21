/*!
 * \file FVAFlowController.cpp
 * \copyright Copyright 2021 FVA Software. All rights reserved. This file is released under the XXX License.
 * \author Dima Nikulin.
 * \version 0.29
 * \date  2014-2021
 */
#include "FVAFlowController.h"

#include <QtCore/QCoreApplication>
#include <QtCore/QDir>
#include <QtCore/QProcess>
#include <QtCore/QStringList>

#include "fvacommoncsv.h"
#include "fvacommonui.h"
#include "fvaconfiguration.h"
#include "fvalogger.inl"

FVAFlowController::FVAFlowController() {
    FVA_EXIT_CODE exitCode = m_cfg.load(QCoreApplication::applicationDirPath() + "/fvaParams.csv");

    // show error message box and return if previous operation failed
    IF_CLT_ERROR_SHOW_MSG_BOX_AND_RET("cfg.load")
}

FVA_EXIT_CODE FVAFlowController::performDeviceChecks(DeviceContext& deviceContext, CLTContext& context) {
    QString rootSWdir;
    FVA_EXIT_CODE exitCode = m_cfg.getParamAsString("Common::RootDir", rootSWdir);

    // show error message box and return to calling function if previous operation failed
    IF_CLT_ERROR_SHOW_MSG_BOX_AND_RET_EXITCODE("getParamAsString(Common::RootDir)")

    context.cmdType = "CLTCheckDeviceName";
    exitCode = m_dataProcessor.run(context, m_cfg);
    if (FVA_ERROR_NON_UNIQUE_DEVICE_NAME == exitCode) {
        context.cmdType = "CLTCreateDirStructByDeviceName";
        exitCode = m_dataProcessor.run(context, m_cfg);
        FVA_MESSAGE_BOX("Found several devices in a folder, please select other dir!");
        return exitCode;
    }
    // show error message box and return to calling function if previous operation failed
    else
        IF_CLT_ERROR_SHOW_MSG_BOX_AND_RET_EXITCODE("CLTCheckDeviceName")

    exitCode = fvaLoadDeviceMapFromCsv(rootSWdir, deviceContext.fullDeviceMap);

    // show error message box and return to calling function if previous operation failed
    IF_CLT_ERROR_SHOW_MSG_BOX_AND_RET_EXITCODE("fvaLoadDeviceMapFromCsv")

    PEOPLE_MAP peopleMap;
    exitCode = fvaLoadPeopleMapFromCsv(rootSWdir, peopleMap);

    // show error message box and return to calling function if previous operation failed
    IF_CLT_ERROR_SHOW_MSG_BOX_AND_RET_EXITCODE("fvaLoadPeopleMapFromCsv")

    for (auto it = deviceContext.fullDeviceMap.begin(); it != deviceContext.fullDeviceMap.end(); ++it)
        it->second.ownerName = peopleMap[it->second.ownerId].name;

    deviceContext.matchedDeviceName.clear();

    QDir _dir(context.dir);
    Q_FOREACH (QFileInfo info, _dir.entryInfoList(QDir::System | QDir::Hidden | QDir::Files, QDir::DirsLast)) {
        if (info.isDir()) continue;
        QString suffix = info.suffix().toUpper();
        FVA_FS_TYPE type = fvaConvertFileExt2FileType(suffix);
        if (FVA_FS_TYPE_IMG != type) continue;

        QString matchedDeviceName;
        deviceContext.deviceMap =
            fvaGetDeviceMapForImg(deviceContext.fullDeviceMap, info.filePath(), matchedDeviceName);
        if (!matchedDeviceName.isEmpty()) {
            deviceContext.matchedDeviceName = matchedDeviceName.toStdString();
            break;
        }
    }
    return FVA_NO_ERROR;
}

void FVAFlowController::performOrientationChecks(const std::string& dir, QObject* obj) {
    const QString dir_ = QString::fromStdString(dir);
    // to run change orentation in auto mode
    QProcess myProcess(obj);
    myProcess.setProcessChannelMode(QProcess::MergedChannels);
    myProcess.start(QCoreApplication::applicationDirPath() + "/jpegr/jpegr.exe -auto " + dir_);
    myProcess.waitForFinished(-1);
}

FVA_EXIT_CODE FVAFlowController::performCommonChecks(CLTContext& context) {
    context.cmdType = "CLTCheckFileFormat";
    FVA_EXIT_CODE exitCode = m_dataProcessor.run(context, m_cfg);

    // show error message box and return to calling function if previous operation failed
    IF_CLT_ERROR_SHOW_MSG_BOX_AND_RET_EXITCODE("CLTCheckFileFormat")

    context.cmdType = "CLTRenameVideoBySequence";
    exitCode = m_dataProcessor.run(context, m_cfg);

    // show error message box and return to calling function if previous operation failed
    IF_CLT_ERROR_SHOW_MSG_BOX_AND_RET_EXITCODE("CLTRenameVideoBySequence")

    context.cmdType = "CLTAutoChecks1";
    exitCode = m_dataProcessor.run(context, m_cfg);

    // show error message box and return to calling function if previous operation failed
    IF_CLT_ERROR_SHOW_MSG_BOX_AND_RET_EXITCODE("CLTAutoChecks1")

    context.cmdType = "CLTSetFileAtts";
    exitCode = m_dataProcessor.run(context, m_cfg);

    // show error message box and return to calling function if previous operation failed
    IF_CLT_ERROR_SHOW_MSG_BOX_AND_RET_EXITCODE("CLTSetFileAtts")

    return FVA_NO_ERROR;
}

FVA_EXIT_CODE FVAFlowController::PerformChecksForInputDir(const std::string& dir, DeviceContext& deviceContext,
                                                          QObject* obj) {
    const QString dir_ = QString::fromStdString(dir);
    // create command-line-task context to keep common parameters for all commands
    CLTContext context;

    // set up the directory that user selected in UI
    context.dir = dir_;

    // perform common checks
    FVA_EXIT_CODE res = performCommonChecks(context);

    // return to calling function if previous operation failed
    RET_RES_IF_RES_IS_ERROR

    // do we need to search by author?
    bool SearchByAuthor = false;

    // ask configuration if we need to search by author
    FVA_EXIT_CODE exitCode = m_cfg.getParamAsBoolean("Search::Author", SearchByAuthor);

    // show error message box and return to calling function if previous operation failed
    IF_CLT_ERROR_SHOW_MSG_BOX_AND_RET_EXITCODE("getParamAsBoolean(Search::Author)")
    if (SearchByAuthor) {
        // perform device checks as author depends on device recognicion
        FVA_EXIT_CODE res = performDeviceChecks(deviceContext, context);

        // return to calling function if previous operation failed
        RET_RES_IF_RES_IS_ERROR
    }

    // do we need to search by date-time?
    bool SearchByDateTime = false;

    // ask configuration if we need to search by date-time
    exitCode = m_cfg.getParamAsBoolean("Search::DateTime", SearchByDateTime);

    // show error message box and return to calling function if previous operation failed
    IF_CLT_ERROR_SHOW_MSG_BOX_AND_RET_EXITCODE("getParamAsBoolean(Search::DateTime)")
    if (SearchByDateTime) {
        // perform date-time checks
        FVA_EXIT_CODE res = performDTChecks(context, obj);

        // return to calling function if previous operation failed
        RET_RES_IF_RES_IS_ERROR
    }

    // do we need to check photo orientation?
    bool needCheckOrientation = false;

    // ask configuration if we need to check orientation
    exitCode = m_cfg.getParamAsBoolean("Common::CheckOrientation", needCheckOrientation);

    // show error message box and return to calling function if previous operation failed
    IF_CLT_ERROR_SHOW_MSG_BOX_AND_RET_EXITCODE("getParamAsBoolean with Common::CheckOrientation")

    // do we need to check photo orientation?
    if (needCheckOrientation)
        // perform orientation checks
        performOrientationChecks(dir, obj);

    return FVA_NO_ERROR;
}

FVA_EXIT_CODE FVAFlowController::runPythonCMD(const std::string& scriptName, QObject* obj,
                                              const std::vector<std::string>& params) {
    const QString scriptName_ = QString::fromStdString(scriptName);
    QString fvaSWRootDir;
    FVA_EXIT_CODE exitCode = m_cfg.getParamAsString("Common::RootDir", fvaSWRootDir);

    // show error message box and return to calling function if previous operation failed
    IF_CLT_ERROR_SHOW_MSG_BOX_AND_RET_EXITCODE("cfg.getParamAsString");

    QString pyScriptRunPath = fvaSWRootDir + "#scripts#/" + scriptName_;

    QStringList params_;
    for (auto it = params.begin(); it != params.end(); ++it) params_.push_back(QString::fromStdString(*it));
    params_.push_front(pyScriptRunPath);

    QProcess myProcess(obj);
    myProcess.setProcessChannelMode(QProcess::MergedChannels);
    myProcess.start("python", params_);
    LOG_DEB << "runPythonCMD:"
            << "pyScriptRunPath=" << pyScriptRunPath;

    myProcess.waitForFinished();

    int exitCode_ = myProcess.exitCode();
    if (exitCode_ != 0) {
        LOG_DEB << "runPythonCMD:"
                << "exitCode_=" << exitCode_;
        return FVA_ERROR_CANT_START_PYTHON_PROC;
    }

    return FVA_NO_ERROR;
}

FVA_EXIT_CODE FVAFlowController::performDTChecks(CLTContext& context, QObject* obj) {
    // prepare context to run CheckDataTime command
    context.cmdType = "CLTCheckDateTime";

    // run CheckDataTime command in Data Proccessor
    FVA_EXIT_CODE exitCode = m_dataProcessor.run(context, m_cfg);

    // lets check if Data Proccessor said there is no exif date time
    if (FVA_ERROR_NO_EXIF_DATE_TIME == exitCode) {
        // let's ask configuration if we can fixing "no exif date time" issue by picture modification time
        bool fixPicsByModifTime = false;
        exitCode = m_cfg.getParamAsBoolean("Rename::picsByModifTime", fixPicsByModifTime);

        // show error message box and return to calling function if previous operation failed
        IF_CLT_ERROR_SHOW_MSG_BOX_AND_RET_EXITCODE("cfg.getParamAsBoolean")

        // if we can NOT fixing "no exif date time" issue by picture modification time
        if (false == fixPicsByModifTime) {
            // show error to user so they are aware what happened
            FVA_MESSAGE_BOX("Found empty date-time metadata, automated fixing is not possible")
            return FVA_ERROR_NOT_IMPLEMENTED;
        } else {
            // show message to user so they are aware what happened
            FVA_MESSAGE_BOX("Found empty date-time metadata, that will be fixed automatically")
        }

        // run command implemented in python to fixing empty date-time issue
        std::vector<std::string> params;
        params.push_back(context.dir.toStdString());
        exitCode = runPythonCMD("CLTFixEmptyDateTime.py", obj, params);

        // show error message box and return to calling function if previous operation failed
        IF_CLT_ERROR_SHOW_MSG_BOX_AND_RET_EXITCODE("CLTFixEmptyDateTime")
    } else
        // show error message box and return to calling function if previous operation failed
        IF_CLT_ERROR_SHOW_MSG_BOX_AND_RET_EXITCODE("CLTCheckDateTime")

    return FVA_NO_ERROR;
}

FVA_EXIT_CODE FVAFlowController::OrganizeInputDir(const std::string& dir, int deviceId) {
    const QString dir_ = QString::fromStdString(dir);
    CLTContext context;
    context.dir = dir_;
    context.cmdType = "CLTRenameFiles";
    context.readOnly = true;  // in read only mode CLTRenameFiles just checks if renaming is possible
    FVA_EXIT_CODE exitCode = m_dataProcessor.run(context, m_cfg);

    // show error message box and return to calling function if previous operation failed
    IF_CLT_ERROR_SHOW_MSG_BOX_AND_RET_EXITCODE("CLTRenameFiles RO mode")

    context.readOnly = false;
    exitCode = m_dataProcessor.run(context, m_cfg);

    // show error message box and return to calling function if previous operation failed
    IF_CLT_ERROR_SHOW_MSG_BOX_AND_RET_EXITCODE("CLTRenameFiles")

    context.cmdType = "CLTCSVFvaFile";
    context.custom = QString::number(deviceId);
    exitCode = m_dataProcessor.run(context, m_cfg);

    // show error message box and return to calling function if previous operation failed
    IF_CLT_ERROR_SHOW_MSG_BOX_AND_RET_EXITCODE("CLTCSVFvaFile")
    context.custom = "";

    context.cmdType = "CLTCreateDirStructByFileNames";
    exitCode = m_dataProcessor.run(context, m_cfg);

    // show error message box and return to calling function if previous operation failed
    IF_CLT_ERROR_SHOW_MSG_BOX_AND_RET_EXITCODE("CLTCreateDirStructByFileNames")

    context.cmdType = "CLTMoveAloneFiles";
    exitCode = m_dataProcessor.run(context, m_cfg);

    // show error message box and return to calling function if previous operation failed
    IF_CLT_ERROR_SHOW_MSG_BOX_AND_RET_EXITCODE("CLTMoveAloneFiles")

    context.cmdType = "CLTAutoChecks2";
    exitCode = m_dataProcessor.run(context, m_cfg);

    // show error message box and return to calling function if previous operation failed
    IF_CLT_ERROR_SHOW_MSG_BOX_AND_RET_EXITCODE("CLTAutoChecks2")

    return FVA_NO_ERROR;
}

FVA_EXIT_CODE FVAFlowController::ProcessInputDirForPlaces(const DIR_2_ID_MAP& placeMap, QObject* obj) {
    QString fvaSWRootDir;
    FVA_EXIT_CODE exitCode = m_cfg.getParamAsString("Common::RootDir", fvaSWRootDir);

    // show error message box and return to calling function if previous operation failed
    IF_CLT_ERROR_SHOW_MSG_BOX_AND_RET_EXITCODE("cfg.getParamAsString");

    QString fvafileNPath = fvaSWRootDir + "#data#/fvafileN.csv";

    // for each folder in input dir map
    for (auto it = placeMap.begin(); it != placeMap.end(); ++it) {
        std::vector<std::string> params;
        params.push_back(fvafileNPath.toStdString());

        QString fsPath = QString::fromStdString(it->first);
        QFileInfo fi(fsPath);
        if (fi.isDir()) {
            params.push_back(fsPath.toStdString());

            QString placeId = QString::number(it->second);
            params.push_back(placeId.toStdString());

            // run command implemented in python to update the fvafile.csv for each file in folder with placeid  we got
            exitCode = runPythonCMD("CLTUpdatePlaceForDir.py", obj, params);

            LOG_DEB << "CLTUpdatePlaceForDir:" << fvafileNPath << " " << fsPath << " " << placeId;
        }
        if (fi.isFile()) {
            return FVA_ERROR_NOT_IMPLEMENTED;
        }
    }
    // clean up after processing
    QFile::remove(fvaSWRootDir + "#data#/FVA_ERROR_NO_EXIF_LOCATION.csv");

    return FVA_NO_ERROR;
}

FVA_EXIT_CODE FVAFlowController::ProcessInputDirForEvents(const std::string& inputDir, const DIR_2_ID_MAP& eventMap,
                                                          const DIR_2_IDS_MAP& peopleMap, QObject* obj) {
    const QString inputDir_ = QString::fromStdString(inputDir);
    QString fvaSWRootDir;
    FVA_EXIT_CODE exitCode = m_cfg.getParamAsString("Common::RootDir", fvaSWRootDir);

    // show error message box and return to calling function if previous operation failed
    IF_CLT_ERROR_SHOW_MSG_BOX_AND_RET_EXITCODE("cfg.getParamAsString");

    QString fvafileNPath = fvaSWRootDir + "#data#/fvafileN.csv";

    // for each folder in input dir map
    for (auto it = eventMap.begin(); it != eventMap.end(); ++it) {
        std::vector<std::string> params;
        params.push_back(fvafileNPath.toStdString());

        QString fsPath = QString::fromStdString(it->first);
        QFileInfo fi(fsPath);
        if (fi.isDir()) {
            params.push_back(fsPath.toStdString());

            QString eventId = QString::number(it->second);
            params.push_back(eventId.toStdString());

            // run command implemented in python to update the fvafile.csv for each file in folder with eventid  we got
            exitCode = runPythonCMD("CLTUpdateEventForDir.py", obj, params);

            LOG_DEB << "CLTUpdateEventForDir:" << fvafileNPath << " " << fsPath << " " << eventId;

            // show error message box and return to calling function if previous operation failed
            IF_CLT_ERROR_SHOW_MSG_BOX_AND_RET_EXITCODE("CLTUpdateEventForDir")

            auto peopleIt = peopleMap.find(it->first);
            if (peopleIt == peopleMap.end() || 0 == peopleIt->second.size()) {
                LOG_WARN << "empty people list for " << fvafileNPath << " " << fsPath;
                continue;
            }
            params.pop_back();  // remove last param as it was for previous cmd actual only
            QString peopleIds;
            for (int i = 0; i < peopleIt->second.size(); ++i) {
                peopleIds += QString::number(peopleIt->second[i]);
                if (i < peopleIt->second.size() - 1) peopleIds += ",";
            }
            params.push_back(peopleIds.toStdString());

            LOG_DEB << "CLTUpdateEventPeopleForDir:" << fvafileNPath << " " << fsPath << " " << eventId;

            // run command implemented in python to update the fvafile.csv for each file in folder with event people ids
            // we got
            exitCode = runPythonCMD("CLTUpdateEventPeopleForDir.py", obj, params);

            // show error message box and return to calling function if previous operation failed
            IF_CLT_ERROR_SHOW_MSG_BOX_AND_RET_EXITCODE("CLTUpdateEventPeopleForDir")
        }
        if (fi.isFile()) {
            return FVA_ERROR_NOT_IMPLEMENTED;
        }
    }

    // do we need to search by place?
    bool SearchByPlace = false;

    // ask configuration if we need to search by place
    exitCode = m_cfg.getParamAsBoolean("Search::Place", SearchByPlace);

    // show error message box and return to calling function if previous operation failed
    IF_CLT_ERROR_SHOW_MSG_BOX_AND_RET_EXITCODE("getParamAsBoolean(Search::Place)")

    CLTContext context;  // empty so far
    context.dir = inputDir_;

    if (SearchByPlace) {
        // we will check if location is not empty so
        context.cmdType = "CLTCheckLocation";
        // in write  mode CLTCheckLocation checks whole content
        // if GPS coordinates are present and creates csv file with list of files with empty GPS coordinates
        context.readOnly = false;
        exitCode = m_dataProcessor.run(context, m_cfg);

        // show error message box and return to calling function if previous operation failed
        IF_CLT_ERROR_SHOW_MSG_BOX_AND_RET_EXITCODE(context.cmdType)
    }
    return FVA_NO_ERROR;
}

FVA_EXIT_CODE FVAFlowController::GetProblemFilesList(STR_LIST& fileListToFillUp) {
    QString rootSWdir;
    FVA_EXIT_CODE exitCode = m_cfg.getParamAsString("Common::RootDir", rootSWdir);

    // show error message box and return to calling function if previous operation failed
    IF_CLT_ERROR_SHOW_MSG_BOX_AND_RET_EXITCODE("cfg.getParamAsString");

    return fvaLoadStrListFromFile(rootSWdir + "#data#/FVA_ERROR_NO_EXIF_LOCATION.csv", fileListToFillUp);
}

FVA_EXIT_CODE FVAFlowController::UpdateInputDirContent(const std::string& inputDir, QObject* obj) {
    LOG_DEB << "Enter";
    const QString inputDir_ = QString::fromStdString(inputDir);

    CLTContext context;
    context.dir = inputDir_;
    context.cmdType = "CLTCSVGetTagsForFvaFiles";
    FVA_EXIT_CODE exitCode = m_dataProcessor.run(context, m_cfg);

    // show error message box and return to calling function if previous operation failed
    IF_CLT_ERROR_SHOW_MSG_BOX_AND_RET_EXITCODE(context.cmdType)

    LOG_DEB << "Exit";
    return FVA_NO_ERROR;
}

FVA_EXIT_CODE FVAFlowController::MoveInputDirToOutputDirs(const std::string& inputDir, const STR_LIST& outputDirs,
                                                          bool removeInput, QObject* obj) {
    LOG_DEB << "Enter";
    const QString inputDir_ = QString::fromStdString(inputDir);

    // get the size of folder list we received
    uint sizeProcessed = outputDirs.size();

    CLTContext context;  // empty so far
    context.dir = inputDir_;
    context.cmdType = "CLTMoveInputDir2Output";

    // for each folder in output list
    for (STR_LIST::const_iterator it = outputDirs.begin(); it != outputDirs.end(); ++it) {
        QString dirToMoveTo = QString::fromStdString(*it);
        context.outputDir = dirToMoveTo;
        LOG_DEB << "Moving into:" << dirToMoveTo;

        // check if we got 1 folder only to integrate the multimedia data  into
        // and if we need to remove the input folder as well
        if (1 == sizeProcessed && removeInput)
        // in read only it moves the content
        {
            context.readOnly = true;
            LOG_DEB << "Moving Content";
        } else {
            // otherwise it copies content (if readonly is NO)
            context.readOnly = false;
            LOG_DEB << "Copying Content";
        }
        context.custom = "merge";
        FVA_EXIT_CODE exitCode = m_dataProcessor.run(context, m_cfg);

        if (exitCode == FVA_ERROR_DEST_DIR_ALREADY_EXISTS) {
            // ask user for what to do
            // TODO to make multilanguage
            QMessageBox msgBox;
            msgBox.setText("The sub directory for merge is already present in target!");
            msgBox.setInformativeText(
                "By pressing OK all the content will be merged to already existing folder,\n by pressing Cancel new "
                "dir will be created to keep the content");
            msgBox.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
            msgBox.setDefaultButton(QMessageBox::Ok);

            context.custom = (msgBox.exec() == QMessageBox::Ok) ? "merge" : "create";
        }

        // and run it again
        exitCode = m_dataProcessor.run(context, m_cfg);

        if (FVA_ERROR_DEST_FILE_ALREADY_EXISTS == exitCode) {
            CLTContext contextDupl;  // empty so far
            contextDupl.dir = inputDir_;
            contextDupl.cmdType = "CLTFixDuplicatedFileNames";

            // run new cmd
            exitCode = m_dataProcessor.run(contextDupl, m_cfg);

            // show error message box and return to calling function if previous operation failed
            IF_CLT_ERROR_SHOW_MSG_BOX_AND_RET_EXITCODE(contextDupl.cmdType)

            // and run move cmd again
            exitCode = m_dataProcessor.run(context, m_cfg);
        }
        // show error message box and return to calling function if previous operation failed
        IF_CLT_ERROR_SHOW_MSG_BOX_AND_RET_EXITCODE(context.cmdType)
    }

    QString fvaSWRootDir;
    FVA_EXIT_CODE exitCode = m_cfg.getParamAsString("Common::RootDir", fvaSWRootDir);

    // show error message box and return to calling function if previous operation failed
    IF_CLT_ERROR_SHOW_MSG_BOX_AND_RET_EXITCODE("cfg.getParamAsString");

    std::vector<std::string> params;
    params.push_back((fvaSWRootDir + "#data#/fvaFile.csv").toStdString());
    params.push_back((fvaSWRootDir + "#data#/fvaFileN.csv").toStdString());

    // run command implemented in python to the fvafile.csv and fvafileN.csv
    exitCode = runPythonCMD("CLTMerge2csv.py", obj, params);

    // show error message box and return to calling function if previous operation failed
    IF_CLT_ERROR_SHOW_MSG_BOX_AND_RET_EXITCODE(context.cmdType)

    // clean up after processing
    QFile::remove(fvaSWRootDir + "#data#/fvaFileN.csv");

    // TODO - call CLTAutoChecks3 for all merged folders

    return FVA_NO_ERROR;
}
