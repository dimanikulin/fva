/*!
* \file FVAFlowController.cpp
* \copyright Copyright 2021 FVA Software. All rights reserved. This file is released under the XXX License.
* \author Dima Nikulin.
* \version 0.29
* \date  2014-2021
*/
#include "FVAFlowController.h"

#include <QtCore/QProcess>
#include <QtCore/QCoreApplication>
#include <QtCore/QDir>

#include "fvaconfiguration.h"
#include "fvacommonui.h"
#include "fvacommoncsv.h"
#include "fvalogger.inl"

FVAFlowController::FVAFlowController()
{
	FVA_EXIT_CODE exitCode = m_cfg.load(QCoreApplication::applicationDirPath() + "/fvaParams.csv");

	// show error message box and return if previous operation failed
	IF_CLT_ERROR_SHOW_MSG_BOX_AND_RET("cfg.load")
}

FVA_EXIT_CODE FVAFlowController::performDeviceChecks(DeviceContext& deviceContext, CLTContext& context, const FvaConfiguration& cfg)
{
	QString rootSWdir;
	FVA_EXIT_CODE exitCode = cfg.getParamAsString("Common::RootDir", rootSWdir);
	
	// show error message box and return to calling function if previous operation failed
	IF_CLT_ERROR_SHOW_MSG_BOX_AND_RET_EXITCODE("getParamAsString(Common::RootDir)")

	context.cmdType = "CLTCheckDeviceName";
	exitCode = m_dataProcessor.run(context, cfg);
	if (FVA_ERROR_NON_UNIQUE_DEVICE_NAME == exitCode)
	{
		context.cmdType = "CLTCreateDirStructByDeviceName";
		exitCode = m_dataProcessor.run(context, cfg);
		FVA_MESSAGE_BOX("Found several devices in a folder, please select other dir!");
		return exitCode;
	}
	// show error message box and return to calling function if previous operation failed
	else IF_CLT_ERROR_SHOW_MSG_BOX_AND_RET_EXITCODE("CLTCheckDeviceName")

	exitCode = fvaLoadDeviceMapFromCsv(rootSWdir, deviceContext.fullDeviceMap);
	
	// show error message box and return to calling function if previous operation failed
	IF_CLT_ERROR_SHOW_MSG_BOX_AND_RET_EXITCODE("fvaLoadDeviceMapFromCsv")

	PEOPLE_MAP peopleMap;
	exitCode = fvaLoadPeopleMapFromCsv(rootSWdir, peopleMap);
	
	// show error message box and return to calling function if previous operation failed
	IF_CLT_ERROR_SHOW_MSG_BOX_AND_RET_EXITCODE("fvaLoadPeopleMapFromCsv")

	for (auto it = deviceContext.fullDeviceMap.begin(); it != deviceContext.fullDeviceMap.end(); ++it)
		it.value().ownerName = peopleMap[it.value().ownerId].name;

	QDir _dir(context.dir);
	Q_FOREACH(QFileInfo info, _dir.entryInfoList(QDir::System | QDir::Hidden | QDir::Files, QDir::DirsLast))
	{
		if (info.isDir())
			continue;
		QString suffix = info.suffix().toUpper();
		FVA_FS_TYPE type = fvaConvertFileExt2FileType(suffix);
		if (FVA_FS_TYPE_IMG != type)
			continue;

		deviceContext.deviceMap = fvaGetDeviceMapForImg(deviceContext.fullDeviceMap, info.filePath(), deviceContext.matchedDeviceName);
		if (!deviceContext.matchedDeviceName.isEmpty())
			break;
	}
	return FVA_NO_ERROR;
}
void FVAFlowController::performOrientationChecks(const QString& dir, QObject* obj)
{
	// to run change orentation in auto mode
	QProcess myProcess(obj);
	myProcess.setProcessChannelMode(QProcess::MergedChannels);
	myProcess.start(QCoreApplication::applicationDirPath() + "/jpegr/jpegr.exe -auto " + dir);
	myProcess.waitForFinished(-1);
}

FVA_EXIT_CODE FVAFlowController::performCommonChecks(CLTContext& context, const FvaConfiguration& cfg)
{
	context.cmdType = "CLTCheckFileFormat";
	FVA_EXIT_CODE exitCode = m_dataProcessor.run(context, cfg);

	// show error message box and return to calling function if previous operation failed
	IF_CLT_ERROR_SHOW_MSG_BOX_AND_RET_EXITCODE("CLTCheckFileFormat")

	context.cmdType = "CLTRenameVideoBySequence";
	exitCode = m_dataProcessor.run(context, cfg);

	// show error message box and return to calling function if previous operation failed
	IF_CLT_ERROR_SHOW_MSG_BOX_AND_RET_EXITCODE("CLTRenameVideoBySequence")

	context.cmdType = "CLTAutoChecks1";
	exitCode = m_dataProcessor.run(context, cfg);

	// show error message box and return to calling function if previous operation failed
	IF_CLT_ERROR_SHOW_MSG_BOX_AND_RET_EXITCODE("CLTAutoChecks1")

	context.cmdType = "CLTSetFileAtts";
	exitCode = m_dataProcessor.run(context, cfg);

	// show error message box and return to calling function if previous operation failed
	IF_CLT_ERROR_SHOW_MSG_BOX_AND_RET_EXITCODE("CLTSetFileAtts")

	return FVA_NO_ERROR;
}

FVA_EXIT_CODE FVAFlowController::PerformChecksForInputDir(const QString& dir, DeviceContext& deviceContext, QObject* obj)
{
	// create command-line-task context to keep common parameters for all commands
	CLTContext context;
	
	// set up the directory that user selected in UI
	context.dir = dir;

	// perform common checks
	FVA_EXIT_CODE res = performCommonChecks(context, m_cfg);

	// return to calling function if previous operation failed
	RET_RES_IF_RES_IS_ERROR

	// do we need to search by device?
	bool SearchByDevice = false;

	// ask configuration if we need to search by device
	FVA_EXIT_CODE exitCode = m_cfg.getParamAsBoolean("Search::Device", SearchByDevice);

	// show error message box and return to calling function if previous operation failed
	IF_CLT_ERROR_SHOW_MSG_BOX_AND_RET_EXITCODE("getParamAsBoolean(Search::Device)")
	if (SearchByDevice)
	{
		// perform device checks
		FVA_EXIT_CODE res = performDeviceChecks(deviceContext, context, m_cfg);

		// return to calling function if previous operation failed
		RET_RES_IF_RES_IS_ERROR
	}

	// do we need to search by date-time?
	bool SearchByDateTime = false;

	// ask configuration if we need to search by date-time
	exitCode = m_cfg.getParamAsBoolean("Search::DateTime", SearchByDateTime);

	// show error message box and return to calling function if previous operation failed
	IF_CLT_ERROR_SHOW_MSG_BOX_AND_RET_EXITCODE("getParamAsBoolean(Search::DateTime)")
	if (SearchByDateTime)
	{
		// perform date-time checks
		FVA_EXIT_CODE res = performDTChecks(context, m_cfg, obj);

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
		performOrientationChecks(dir,obj);

	return FVA_NO_ERROR;
}
FVA_EXIT_CODE FVAFlowController::runPythonCMD(const QString& scriptName, QObject* obj, const FvaConfiguration& cfg, const QStringList& params)
{
	QString fvaSWRootDir;
	FVA_EXIT_CODE exitCode = cfg.getParamAsString("Common::RootDir", fvaSWRootDir);

	// show error message box and return to calling function if previous operation failed
	IF_CLT_ERROR_SHOW_MSG_BOX_AND_RET_EXITCODE("cfg.getParamAsString");

	QString pyScriptRunPath = "python " + fvaSWRootDir + "/#scripts#/" + scriptName;

	QProcess myProcess(obj);
	myProcess.setProcessChannelMode(QProcess::MergedChannels);
	myProcess.start(pyScriptRunPath, params);
	myProcess.waitForFinished(-1);

	exitCode = static_cast<FVA_EXIT_CODE> (myProcess.exitCode());
	
	// show error message box and return to calling function if previous operation failed
	IF_CLT_ERROR_SHOW_MSG_BOX_AND_RET_EXITCODE(pyScriptRunPath);
	return FVA_NO_ERROR;
}
FVA_EXIT_CODE FVAFlowController::performDTChecks(CLTContext& context, const FvaConfiguration& cfg, QObject* obj)
{
	// prepare context to run CheckDataTime command
	context.cmdType = "CLTCheckDateTime";

	// run CheckDataTime command in Data Proccessor
	FVA_EXIT_CODE exitCode = m_dataProcessor.run(context, cfg);

	// lets check if Data Proccessor said there is no exif date time 
	if (FVA_ERROR_NO_EXIF_DATE_TIME == exitCode)
	{
		// let's ask configuration if we can fixing "no exif date time" issue by picture modification time 
		bool fixPicsByModifTime = false;
		exitCode = cfg.getParamAsBoolean("Rename::picsByModifTime", fixPicsByModifTime);

		// show error message box and return to calling function if previous operation failed
		IF_CLT_ERROR_SHOW_MSG_BOX_AND_RET_EXITCODE("cfg.getParamAsBoolean")

		// if we can NOT fixing "no exif date time" issue by picture modification time 
		if (false == fixPicsByModifTime)
		{
			// show error to user so they are aware what happened
			FVA_MESSAGE_BOX("Found empty date-time metadata, automated fixing is not possible")
			return FVA_ERROR_NOT_IMPLEMENTED;
		}
		else
		{
			// show message to user so they are aware what happened
			FVA_MESSAGE_BOX("Found empty date-time metadata, that will be fixed automatically")
		}

		// run command implemented in python to fixing empty date-time issue
		QStringList params;
		params.append(context.dir);
		exitCode = runPythonCMD("CLTFixEmptyDateTime.py", obj, cfg, params);

		// show error message box and return to calling function if previous operation failed
		IF_CLT_ERROR_SHOW_MSG_BOX_AND_RET_EXITCODE("CLTFixEmptyDateTime")

	}
	else
		// show error message box and return to calling function if previous operation failed
		IF_CLT_ERROR_SHOW_MSG_BOX_AND_RET_EXITCODE("CLTCheckDateTime")

	return FVA_NO_ERROR;
}

FVA_EXIT_CODE FVAFlowController::performLocationChecks(CLTContext& context, const FvaConfiguration& cfg)
{
	context.cmdType = "CLTCheckLocation";
	FVA_EXIT_CODE exitCode = m_dataProcessor.run(context, cfg);

	// show error message box and return to calling function if previous operation failed
	IF_CLT_ERROR_SHOW_MSG_BOX_AND_RET_EXITCODE("CLTCheckLocation")
	return FVA_NO_ERROR;
}
FVA_EXIT_CODE FVAFlowController::OrganizeInputDir(const QString& dir, int deviceId)
{
	CLTContext context;
	context.dir = dir;
	context.cmdType = "CLTRenameFiles";
	context.readOnly = true; // in read only mode CLTRenameFiles just checks if renaming is possible 
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

FVA_EXIT_CODE FVAFlowController::ProcessInputDirForEvent(const DIR_2_EVENT_MAP& eventMap, const DIR_2_EVENT_PEOPLE_MAP& peopleMap, QObject* obj)
{
	QString fvaSWRootDir;
	FVA_EXIT_CODE exitCode = m_cfg.getParamAsString("Common::RootDir", fvaSWRootDir);

	// show error message box and return to calling function if previous operation failed
	IF_CLT_ERROR_SHOW_MSG_BOX_AND_RET_EXITCODE("cfg.getParamAsString");

	QStringList params;
	params.append(fvafileNPath);

	QString fvafileNPath = fvaSWRootDir + "/#data#/fvafileN.csv";

	// for each folder in output list
	for (auto it = eventMap.begin(); it != eventMap.end(); ++it)
	{		
		QString dir = it.key();
		params.append(dir);

		QString eventId = QString::number(it.value());
		params.append(eventId);
	
		// run command implemented in python to update the fvafile.csv for each file in folder with eventid  we got 
		exitCode = runPythonCMD("CLTUpdateEventInFvaFile.py", obj, m_cfg, params);

		// show error message box and return to calling function if previous operation failed
		IF_CLT_ERROR_SHOW_MSG_BOX_AND_RET_EXITCODE("CLTUpdateEventInFvaFile")

		params.removeLast(); // remove last param as ot was for previous cmd actual only
		QString peopleIds;
		for(int i=0; i < peopleMap[dir].size(); ++i)
		{
			peopleIds += QString::number(peopleMap[dir][i]);
			if( i < peopleMap[dir].size()-1 )
				peopleIds += "," ;
		}
		params.append(peopleIds);

		LOG_DEB << "FVAFlowController::ProcessInputDirForEvent " << fvafileNPath << " " << dir << " " << eventId << " " << peopleIds;

		// run command implemented in python to update the fvafile.csv for each file in folder with event people ids we got 
		exitCode = runPythonCMD("CLTUpdateEventPeopleInFvaFile.py", obj, m_cfg, params);

		// show error message box and return to calling function if previous operation failed
		IF_CLT_ERROR_SHOW_MSG_BOX_AND_RET_EXITCODE("CLTUpdateEventPeopleInFvaFile")
	}

	// do we need to search by location?
	/*bool SearchByLocation = false;

	// ask configuration if we need to search by location
	exitCode = m_cfg.getParamAsBoolean("Search::Location", SearchByLocation);

	// show error message box and return to calling function if previous operation failed
	IF_CLT_ERROR_SHOW_MSG_BOX_AND_RET_EXITCODE("getParamAsBoolean(Search::Location)")
	if (SearchByLocation)
	{
		// perform location checks
		FVA_EXIT_CODE res = performLocationChecks(context, m_cfg);

		// return to calling function if previous operation failed
		RET_RES_IF_RES_IS_ERROR
	}*/
	

	return FVA_NO_ERROR;
}
FVA_EXIT_CODE FVAFlowController::MoveInputDirToOutputDirs(const QString& inputDir, const STR_LIST& outputDirs, bool removeInput)
{
	// get the size of folder list we received
	uint sizeProcessed = outputDirs.size();

	// for each folder in output list
	for (STR_LIST::const_iterator it = outputDirs.begin(); it != outputDirs.end(); ++it)
	{		
		QString dirToMoveTo = *it;
		// check if we got 1 folder only to integrate the multimedia data changes into 
		// and if we need to remove the input folder as well
		if (1 == sizeProcessed && removeInput)
		{
			// remove before rename if destination exists
			fvaRemoveDirIfEmpty(dirToMoveTo);

			// small optimization - do not copy to last folder if we need to remove the input one - we just rename it.
			QDir dir(dirToMoveTo);

			// move the input folder into output one that is what user selected in UI as Multimedia IR system folder
			if (!dir.rename(inputDir, dirToMoveTo))
			{
				// show message if folder moving has failed
				FVA_MESSAGE_BOX("Fva cmd MoveInputDirToOutputDirs could not rename the dir")
				return FVA_ERROR_CANT_MOVE_DIR;
			}
		}
		else
		{
			return FVA_ERROR_NOT_IMPLEMENTED;
		}
	}
	/*
	if (oneEventOneDay->isChecked())
	{
		exitCode = fvaRunCLT("CLTMerge1DayEventDir", ((FVAOrganizerWizard*)wizard())->inputFolder());

		QString mode = "merge";
		if (exitCode == FVA_ERROR_DEST_DIR_ALREADY_EXISTS)
		{
			// ask user for what to do 
			QMessageBox msgBox;
			msgBox.setText("The dir for merge is already present in target!");
			msgBox.setInformativeText("By pressing OK all the content will be merged to already existing folder,\n by pressing Cancel new dir will be created to keep the content");
			msgBox.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
			msgBox.setDefaultButton(QMessageBox::Ok);

			QString mode = (msgBox.exec() == QMessageBox::Ok) ? "merge" : "create";
		}

		// and run it again
		exitCode = fvaRunCLT("CLTMerge1DayEventDir", ((FVAOrganizerWizard*)wizard())->inputFolder(), false, false, mode);
		
		// show error message box and return to calling function if previous operation failed
		IF_CLT_ERROR_SHOW_MSG_BOX_AND_RET_FALSE("CLTMerge1DayEventDir")

		if (FVA_ERROR_DEST_FILE_ALREADY_EXISTS == exitCode)
		{
			exitCode = fvaRunCLT("CLTFixDuplicatedFileNames", ((FVAOrganizerWizard*)wizard())->inputFolder());
			
			// show error message box and return to calling function if previous operation failed
			IF_CLT_ERROR_SHOW_MSG_BOX_AND_RET_FALSE("CLTFixDuplicatedFileNames")
			
			exitCode = fvaRunCLT("CLTMerge1DayEventDir", ((FVAOrganizerWizard*)wizard())->inputFolder());
		}
		// show error message box and return to calling function if previous operation failed
		IF_CLT_ERROR_SHOW_MSG_BOX_AND_RET_FALSE("CLTMerge1DayEventDir")
	}
	else if (oneEventSeveralDays->isChecked())
	{
		exitCode = fvaRunCLT("CLTMerge1EventDir", ((FVAOrganizerWizard*)wizard())->inputFolder(), true, false, outputDirLineEdit->text());

		if (FVA_ERROR_DEST_FILE_ALREADY_EXISTS == exitCode)
		{
			exitCode = fvaRunCLT("CLTFixDuplicatedFileNames", ((FVAOrganizerWizard*)wizard())->inputFolder());
			
			// show error message box and return to calling function if previous operation failed
			IF_CLT_ERROR_SHOW_MSG_BOX_AND_RET_FALSE("CLTFixDuplicatedFileNames")
			
			exitCode = fvaRunCLT("CLTMerge1EventDir", ((FVAOrganizerWizard*)wizard())->inputFolder());
		}
		// show error message box and return to calling function if previous operation failed
		IF_CLT_ERROR_SHOW_MSG_BOX_AND_RET_FALSE("CLTMerge1EventDir")
	}
	else
	{
		FVA_MESSAGE_BOX("Not implemented yet");
		return false;
	}

	FvaConfiguration cfg;
	FVA_EXIT_CODE res = cfg.load(QCoreApplication::applicationDirPath() + "/fvaParams.csv");
	if (FVA_NO_ERROR != res)
	{
		FVA_MESSAGE_BOX("cfg.load failed with error " + QString::number(exitCode));
		return false;
	}

	QString fvaSWRootDir;
	res = cfg.getParamAsString("Common::RootDir", fvaSWRootDir);
	if (FVA_NO_ERROR != res)
	{
		FVA_MESSAGE_BOX("cfg.load failed with error " + QString::number(exitCode));
		return false;
	}

	QStringList pyCmdList;
	// merge 2 csv into one: common one and just generated - for file CSVs
	QString pyScriptPathMerge2 = "python "
		+ QCoreApplication::applicationDirPath()
		+ "/scripts/merge2csv.py "
		+ fvaSWRootDir;
	pyCmdList.append(pyScriptPathMerge2 + "#data#/fvaFile.csv " + fvaSWRootDir + "#data#/fvaFileN.csv ");

	// lets run python cmd list 
	for (auto it = pyCmdList.begin(); it != pyCmdList.end(); ++it)
	{
		QProcess myProcess(this);
		myProcess.setProcessChannelMode(QProcess::MergedChannels);
		myProcess.start(*it);
		myProcess.waitForFinished(-1);

		int exitCode = myProcess.exitCode();
		if (exitCode != 0)
		{
			FVA_MESSAGE_BOX("Fva cmd " + *it + " failed with error " + QString::number(exitCode));
			return false;
		}
	}

	// clean up after processing
	QFile::remove(fvaSWRootDir + "#data#/fvaFileN.csv");

	// last but not least check
	if (oneEventSeveralDays->isChecked())
	{
		exitCode = fvaRunCLT("CLTAutoChecks3", outputDirLineEdit->text());
		// show error message box and return to calling function if previous operation failed
		IF_CLT_ERROR_SHOW_MSG_BOX_AND_RET_FALSE("CLTAutoChecks3")
	}
	else
	{
		// TODO - apply call CLTAutoChecks3 for all merged folders
	}*/

	return FVA_NO_ERROR;
}