/*!
 * \file FVAFlowController.cpp
 * \copyright Copyright 2021 FVA Software. All rights reserved. This file is released under the XXX License.
 * \author Dima Nikulin.
 * \version 0.29
 * \date  2014-2021
 */
#include "FVAFlowController.h"

#include <algorithm>
#include <cctype>
#include <cstdlib>
#include <filesystem>

#ifdef _WIN32
#include <Windows.h>
#endif

#include "fva_qt_port_2_stl.h"
#include "fvacommoncsv.h"
#include "fvaconfiguration.h"
#include "fvalogger.inl"

// TODO replace with with proper implementation of message box
#define FVA_MESSAGE_BOX(msg) \
    do {                     \
        (void)(msg);         \
    } while (0)

#define IF_CLT_ERROR_SHOW_MSG_BOX_AND_RET(msg)                                          \
    if (exitCode != FVA_NO_ERROR) {                                                     \
        FVA_MESSAGE_BOX(std::string("Error happened during ") + (msg) + " operation!"); \
        return;                                                                         \
    }

#define IF_CLT_ERROR_SHOW_MSG_BOX_AND_RET_EXITCODE(msg)                                 \
    if (exitCode != FVA_NO_ERROR) {                                                     \
        FVA_MESSAGE_BOX(std::string("Error happened during ") + (msg) + " operation!"); \
        return exitCode;                                                                \
    }

FVAFlowController::FVAFlowController() {
    const std::string cfgPath = getApplicationDirPath() + "/fvaParams.csv";
    FVA_EXIT_CODE exitCode = m_cfg.load(cfgPath);

    // show error message box and return if previous operation failed
    IF_CLT_ERROR_SHOW_MSG_BOX_AND_RET("cfg.load")
}

FVA_EXIT_CODE FVAFlowController::performDeviceChecks(DeviceContext& deviceContext, CLTContext& context) {
    std::string rootSWdir;
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

    std::error_code fsError;
    for (const auto& entry : std::filesystem::directory_iterator(context.dir, fsError)) {
        if (fsError != std::error_code()) break;
        if (!entry.is_regular_file(fsError)) continue;
        if (fsError != std::error_code()) continue;

        std::string suffix = entry.path().extension().string();
        if (!suffix.empty() && suffix[0] == '.') suffix.erase(0, 1);
        std::transform(suffix.begin(), suffix.end(), suffix.begin(),
                       [](unsigned char c) { return static_cast<char>(std::toupper(c)); });

        FVA_FS_TYPE type = fvaConvertFileExt2FileType(suffix);
        if (FVA_FS_TYPE_IMG != type) continue;

        std::string matchedDeviceName;
        deviceContext.deviceMap =
            fvaGetDeviceMapForImg(deviceContext.fullDeviceMap, entry.path().string(), matchedDeviceName);
        if (!matchedDeviceName.empty()) {
            deviceContext.matchedDeviceName = matchedDeviceName;
            break;
        }
    }
    return FVA_NO_ERROR;
}

void FVAFlowController::performOrientationChecks(const std::string& dir) {
    // to run change orentation in auto mode
    const std::string command = quoteArg(getApplicationDirPath() + "/jpegr/jpegr.exe") + " -auto " + quoteArg(dir);
    std::system(command.c_str());
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

FVA_EXIT_CODE FVAFlowController::PerformChecksForInputDir(const std::string& dir, DeviceContext& deviceContext) {
    // create command-line-task context to keep common parameters for all commands
    CLTContext context;

    // set up the directory that user selected in UI
    context.dir = dir;

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
        FVA_EXIT_CODE res = performDTChecks(context);

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
        performOrientationChecks(dir);

    return FVA_NO_ERROR;
}

FVA_EXIT_CODE FVAFlowController::runPythonCMD(const std::string& scriptName, const std::vector<std::string>& params) {
    std::string fvaSWRootDir;
    FVA_EXIT_CODE exitCode = m_cfg.getParamAsString("Common::RootDir", fvaSWRootDir);

    // show error message box and return to calling function if previous operation failed
    IF_CLT_ERROR_SHOW_MSG_BOX_AND_RET_EXITCODE("cfg.getParamAsString");

    const std::string pyScriptRunPath = fvaSWRootDir + "#scripts#/" + scriptName;

    std::string command = "python " + quoteArg(pyScriptRunPath);
    for (auto it = params.begin(); it != params.end(); ++it) command += " " + quoteArg(*it);

    LOG_DEB << "runPythonCMD:"
            << "pyScriptRunPath=" << pyScriptRunPath;

    int exitCode_ = std::system(command.c_str());
    if (exitCode_ != 0) {
        LOG_DEB << "runPythonCMD:"
                << "exitCode_=" << exitCode_;
        return FVA_ERROR_CANT_START_PYTHON_PROC;
    }

    return FVA_NO_ERROR;
}

FVA_EXIT_CODE FVAFlowController::performDTChecks(CLTContext& context) {
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
        params.push_back(context.dir);
        exitCode = runPythonCMD("CLTFixEmptyDateTime.py", params);

        // show error message box and return to calling function if previous operation failed
        IF_CLT_ERROR_SHOW_MSG_BOX_AND_RET_EXITCODE("CLTFixEmptyDateTime")
    } else
        // show error message box and return to calling function if previous operation failed
        IF_CLT_ERROR_SHOW_MSG_BOX_AND_RET_EXITCODE("CLTCheckDateTime")

    return FVA_NO_ERROR;
}

FVA_EXIT_CODE FVAFlowController::OrganizeInputDir(const std::string& dir, int deviceId) {
    CLTContext context;
    context.dir = dir;
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
    context.custom = std::to_string(deviceId);
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

FVA_EXIT_CODE FVAFlowController::ProcessInputDirForPlaces(const DIR_2_ID_MAP& placeMap) {
    std::string fvaSWRootDir;
    FVA_EXIT_CODE exitCode = m_cfg.getParamAsString("Common::RootDir", fvaSWRootDir);

    // show error message box and return to calling function if previous operation failed
    IF_CLT_ERROR_SHOW_MSG_BOX_AND_RET_EXITCODE("cfg.getParamAsString");

    const std::string fvafileNPath = fvaSWRootDir + "#data#/fvafileN.csv";

    // for each folder in input dir map
    for (auto it = placeMap.begin(); it != placeMap.end(); ++it) {
        std::vector<std::string> params;
        params.push_back(fvafileNPath);

        const std::string& fsPath = it->first;
        std::error_code fsError;
        if (std::filesystem::is_directory(fsPath, fsError)) {
            params.push_back(fsPath);

            const std::string placeId = std::to_string(it->second);
            params.push_back(placeId);

            // run command implemented in python to update the fvafile.csv for each file in folder with placeid  we got
            exitCode = runPythonCMD("CLTUpdatePlaceForDir.py", params);

            LOG_DEB << "CLTUpdatePlaceForDir:" << fvafileNPath << " " << fsPath << " " << placeId;
        }
        if (std::filesystem::is_regular_file(fsPath, fsError)) {
            return FVA_ERROR_NOT_IMPLEMENTED;
        }
    }
    // clean up after processing
    std::remove((fvaSWRootDir + "#data#/FVA_ERROR_NO_EXIF_LOCATION.csv").c_str());

    return FVA_NO_ERROR;
}

FVA_EXIT_CODE FVAFlowController::ProcessInputDirForEvents(const std::string& inputDir, const DIR_2_ID_MAP& eventMap,
                                                          const DIR_2_IDS_MAP& peopleMap) {
    std::string fvaSWRootDir;
    FVA_EXIT_CODE exitCode = m_cfg.getParamAsString("Common::RootDir", fvaSWRootDir);

    // show error message box and return to calling function if previous operation failed
    IF_CLT_ERROR_SHOW_MSG_BOX_AND_RET_EXITCODE("cfg.getParamAsString");

    const std::string fvafileNPath = fvaSWRootDir + "#data#/fvafileN.csv";

    // for each folder in input dir map
    for (auto it = eventMap.begin(); it != eventMap.end(); ++it) {
        std::vector<std::string> params;
        params.push_back(fvafileNPath);

        const std::string& fsPath = it->first;
        std::error_code fsError;
        if (std::filesystem::is_directory(fsPath, fsError)) {
            params.push_back(fsPath);

            const std::string eventId = std::to_string(it->second);
            params.push_back(eventId);

            // run command implemented in python to update the fvafile.csv for each file in folder with eventid  we got
            exitCode = runPythonCMD("CLTUpdateEventForDir.py", params);

            LOG_DEB << "CLTUpdateEventForDir:" << fvafileNPath << " " << fsPath << " " << eventId;

            // show error message box and return to calling function if previous operation failed
            IF_CLT_ERROR_SHOW_MSG_BOX_AND_RET_EXITCODE("CLTUpdateEventForDir")

            auto peopleIt = peopleMap.find(it->first);
            if (peopleIt == peopleMap.end() || 0 == peopleIt->second.size()) {
                LOG_WARN << "empty people list for " << fvafileNPath << " " << fsPath;
                continue;
            }
            params.pop_back();  // remove last param as it was for previous cmd actual only
            std::string peopleIds;
            for (int i = 0; i < peopleIt->second.size(); ++i) {
                peopleIds += std::to_string(peopleIt->second[i]);
                if (i < peopleIt->second.size() - 1) peopleIds += ",";
            }
            params.push_back(peopleIds);

            LOG_DEB << "CLTUpdateEventPeopleForDir:" << fvafileNPath << " " << fsPath << " " << eventId;

            // run command implemented in python to update the fvafile.csv for each file in folder with event people ids
            // we got
            exitCode = runPythonCMD("CLTUpdateEventPeopleForDir.py", params);

            // show error message box and return to calling function if previous operation failed
            IF_CLT_ERROR_SHOW_MSG_BOX_AND_RET_EXITCODE("CLTUpdateEventPeopleForDir")
        }
        if (std::filesystem::is_regular_file(fsPath, fsError)) {
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
    context.dir = inputDir;

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
    std::string rootSWdir;
    FVA_EXIT_CODE exitCode = m_cfg.getParamAsString("Common::RootDir", rootSWdir);

    // show error message box and return to calling function if previous operation failed
    IF_CLT_ERROR_SHOW_MSG_BOX_AND_RET_EXITCODE("cfg.getParamAsString");

    return fvaLoadStrListFromFile(rootSWdir + "#data#/FVA_ERROR_NO_EXIF_LOCATION.csv", fileListToFillUp);
}

FVA_EXIT_CODE FVAFlowController::UpdateInputDirContent(const std::string& inputDir) {
    LOG_DEB << "Enter";

    CLTContext context;
    context.dir = inputDir;
    context.cmdType = "CLTCSVGetTagsForFvaFiles";
    FVA_EXIT_CODE exitCode = m_dataProcessor.run(context, m_cfg);

    // show error message box and return to calling function if previous operation failed
    IF_CLT_ERROR_SHOW_MSG_BOX_AND_RET_EXITCODE(context.cmdType)

    LOG_DEB << "Exit";
    return FVA_NO_ERROR;
}

FVA_EXIT_CODE FVAFlowController::MoveInputDirToOutputDirs(const std::string& inputDir, const STR_LIST& outputDirs,
                                                          bool removeInput) {
    LOG_DEB << "Enter";
    // get the size of folder list we received
    uint sizeProcessed = outputDirs.size();

    CLTContext context;  // empty so far
    context.dir = inputDir;
    context.cmdType = "CLTMoveInputDir2Output";

    // for each folder in output list
    for (STR_LIST::const_iterator it = outputDirs.begin(); it != outputDirs.end(); ++it) {
        const std::string dirToMoveTo = *it;
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
            // TODO - to make it in UI instead of message box and to remove this code from flow controller as it is UI
            // related
            /*QMessageBox msgBox;
            msgBox.setText("The sub directory for merge is already present in target!");
            msgBox.setInformativeText(
                "By pressing OK all the content will be merged to already existing folder,\n by pressing Cancel new "
                "dir will be created to keep the content");
            msgBox.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
            msgBox.setDefaultButton(QMessageBox::Ok);

            context.custom = (msgBox.exec() == QMessageBox::Ok) ? "merge" : "create";*/
        }

        // and run it again
        exitCode = m_dataProcessor.run(context, m_cfg);

        if (FVA_ERROR_DEST_FILE_ALREADY_EXISTS == exitCode) {
            CLTContext contextDupl;  // empty so far
            contextDupl.dir = inputDir;
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

    std::string fvaSWRootDir;
    FVA_EXIT_CODE exitCode = m_cfg.getParamAsString("Common::RootDir", fvaSWRootDir);

    // show error message box and return to calling function if previous operation failed
    IF_CLT_ERROR_SHOW_MSG_BOX_AND_RET_EXITCODE("cfg.getParamAsString");

    std::vector<std::string> params;
    params.push_back(fvaSWRootDir + "#data#/fvaFile.csv");
    params.push_back(fvaSWRootDir + "#data#/fvaFileN.csv");

    // run command implemented in python to the fvafile.csv and fvafileN.csv
    exitCode = runPythonCMD("CLTMerge2csv.py", params);

    // show error message box and return to calling function if previous operation failed
    IF_CLT_ERROR_SHOW_MSG_BOX_AND_RET_EXITCODE(context.cmdType)

    // clean up after processing
    std::remove((fvaSWRootDir + "#data#/fvaFileN.csv").c_str());

    // TODO - call CLTAutoChecks3 for all merged folders

    return FVA_NO_ERROR;
}
