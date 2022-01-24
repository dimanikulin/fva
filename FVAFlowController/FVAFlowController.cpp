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
FVA_EXIT_CODE FVAFlowController::performDeviceChecks(DeviceContext& deviceContext, CLTContext& context)
{
	QString rootSWdir;
	FVA_EXIT_CODE exitCode = m_cfg.getParamAsString("Common::RootDir", rootSWdir);
	
	// show error message box and return to calling function if previous operation failed
	IF_CLT_ERROR_SHOW_MSG_BOX_AND_RET_EXITCODE("getParamAsString(Common::RootDir)")

	context.cmdType = "CLTCheckDeviceName";
	exitCode = m_dataProcessor.run(context, m_cfg);
	if (FVA_ERROR_NON_UNIQUE_DEVICE_NAME == exitCode)
	{
		context.cmdType = "CLTCreateDirStructByDeviceName";
		exitCode = m_dataProcessor.run(context, m_cfg);
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

FVA_EXIT_CODE FVAFlowController::performCommonChecks(CLTContext& context)
{
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

FVA_EXIT_CODE FVAFlowController::PerformChecksForInputDir(const QString& dir, DeviceContext& deviceContext, QObject* obj)
{
	// create command-line-task context to keep common parameters for all commands
	CLTContext context;
	
	// set up the directory that user selected in UI
	context.dir = dir;

	// perform common checks
	FVA_EXIT_CODE res = performCommonChecks(context);

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
	if (SearchByDateTime)
	{
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
		performOrientationChecks(dir,obj);

	return FVA_NO_ERROR;
}
FVA_EXIT_CODE FVAFlowController::runPythonCMD(const QString& scriptName, QObject* obj, const QStringList& params)
{
	QString fvaSWRootDir;
	FVA_EXIT_CODE exitCode = m_cfg.getParamAsString("Common::RootDir", fvaSWRootDir);

	// show error message box and return to calling function if previous operation failed
	IF_CLT_ERROR_SHOW_MSG_BOX_AND_RET_EXITCODE("cfg.getParamAsString");

	QString pyScriptRunPath =  fvaSWRootDir + "#scripts#/" + scriptName;

	QStringList params_ = params;
	params_.push_front(pyScriptRunPath);

	QProcess myProcess(obj);
	myProcess.setProcessChannelMode(QProcess::MergedChannels);
	myProcess.start("python", params_);
	LOG_DEB << "runPythonCMD:" << "pyScriptRunPath=" << pyScriptRunPath;

	myProcess.waitForFinished();

	int exitCode_ = myProcess.exitCode();
	if (exitCode_ != 0)
	{
		LOG_DEB << "runPythonCMD:" << "exitCode_=" << exitCode_;	
		return FVA_ERROR_CANT_START_PYTHON_PROC;
	}

	return FVA_NO_ERROR;
}
FVA_EXIT_CODE FVAFlowController::performDTChecks(CLTContext& context, QObject* obj)
{
	// prepare context to run CheckDataTime command
	context.cmdType = "CLTCheckDateTime";

	// run CheckDataTime command in Data Proccessor
	FVA_EXIT_CODE exitCode = m_dataProcessor.run(context, m_cfg);

	// lets check if Data Proccessor said there is no exif date time 
	if (FVA_ERROR_NO_EXIF_DATE_TIME == exitCode)
	{
		// let's ask configuration if we can fixing "no exif date time" issue by picture modification time 
		bool fixPicsByModifTime = false;
		exitCode = m_cfg.getParamAsBoolean("Rename::picsByModifTime", fixPicsByModifTime);

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
		exitCode = runPythonCMD("CLTFixEmptyDateTime.py", obj, params);

		// show error message box and return to calling function if previous operation failed
		IF_CLT_ERROR_SHOW_MSG_BOX_AND_RET_EXITCODE("CLTFixEmptyDateTime")

	}
	else
		// show error message box and return to calling function if previous operation failed
		IF_CLT_ERROR_SHOW_MSG_BOX_AND_RET_EXITCODE("CLTCheckDateTime")

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
FVA_EXIT_CODE FVAFlowController::ProcessInputDirForPlaces(const DIR_2_ID_MAP& placeMap, QObject* obj)
{
	QString fvaSWRootDir;
	FVA_EXIT_CODE exitCode = m_cfg.getParamAsString("Common::RootDir", fvaSWRootDir);

	// show error message box and return to calling function if previous operation failed
	IF_CLT_ERROR_SHOW_MSG_BOX_AND_RET_EXITCODE("cfg.getParamAsString");

	QStringList params;

	QString fvafileNPath = fvaSWRootDir + "#data#/fvafileN.csv";
	params.append(fvafileNPath);

	// for each folder in input dir map
	for (auto it = placeMap.begin(); it != placeMap.end(); ++it)
	{		
		QString dir = it.key();
		params.append(dir);

		QString placeId = QString::number(it.value());

		// run command implemented in python to update the fvafile.csv for each file in folder with placeid  we got 
		exitCode = runPythonCMD("CLTUpdatePlaceForDir.py", obj, params);

		LOG_DEB << "CLTUpdatePlaceForDir:" << fvafileNPath << " " << dir << " " << placeId;
	}
	return FVA_NO_ERROR;
}
FVA_EXIT_CODE FVAFlowController::ProcessInputDirForEvents(const QString& inputDir, const DIR_2_ID_MAP& eventMap, const DIR_2_IDS_MAP& peopleMap, QObject* obj)
{
	QString fvaSWRootDir;
	FVA_EXIT_CODE exitCode = m_cfg.getParamAsString("Common::RootDir", fvaSWRootDir);

	// show error message box and return to calling function if previous operation failed
	IF_CLT_ERROR_SHOW_MSG_BOX_AND_RET_EXITCODE("cfg.getParamAsString");

	QStringList params;

	QString fvafileNPath = fvaSWRootDir + "#data#/fvafileN.csv";
	params.append(fvafileNPath);

	// for each folder in input dir map
	for (auto it = eventMap.begin(); it != eventMap.end(); ++it)
	{		
		QString dir = it.key();
		params.append(dir);

		QString eventId = QString::number(it.value());
		params.append(eventId);
	
		// run command implemented in python to update the fvafile.csv for each file in folder with eventid  we got 
		exitCode = runPythonCMD("CLTUpdateEventForDir.py", obj, params);

		LOG_DEB << "CLTUpdateEventForDir:" << fvafileNPath << " " << dir << " " << eventId;

		// show error message box and return to calling function if previous operation failed
		IF_CLT_ERROR_SHOW_MSG_BOX_AND_RET_EXITCODE("CLTUpdateEventForDir")

		if (0 == peopleMap[dir].size())
		{
			LOG_WARN << "empty people list for " << fvafileNPath << " " << dir;
			continue;
		}
		params.removeLast(); // remove last param as it was for previous cmd actual only
		QString peopleIds;
		for(int i=0; i < peopleMap[dir].size(); ++i)
		{
			peopleIds += QString::number(peopleMap[dir][i]);
			if( i < peopleMap[dir].size()-1 )
				peopleIds += "," ;
		}
		params.append(peopleIds);

		LOG_DEB << "CLTUpdateEventPeopleForDir:" << fvafileNPath << " " << dir << " " << eventId;

		// run command implemented in python to update the fvafile.csv for each file in folder with event people ids we got 
		exitCode = runPythonCMD("CLTUpdateEventPeopleForDir.py", obj, params);

		// show error message box and return to calling function if previous operation failed
		IF_CLT_ERROR_SHOW_MSG_BOX_AND_RET_EXITCODE("CLTUpdateEventPeopleForDir")
	}

	// do we need to search by location?
	bool SearchByLocation = false;

	// ask configuration if we need to search by location
	exitCode = m_cfg.getParamAsBoolean("Search::Location", SearchByLocation);

	// show error message box and return to calling function if previous operation failed
	IF_CLT_ERROR_SHOW_MSG_BOX_AND_RET_EXITCODE("getParamAsBoolean(Search::Location)")

	CLTContext context; // empty so far
	context.dir = inputDir;

	if (SearchByLocation)
	{
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
FVA_EXIT_CODE FVAFlowController::GetProblemFilesList(STR_LIST& fileListToFillUp)
{
	QString rootSWdir;
	FVA_EXIT_CODE exitCode = m_cfg.getParamAsString("Common::RootDir", rootSWdir);

	// show error message box and return to calling function if previous operation failed
	IF_CLT_ERROR_SHOW_MSG_BOX_AND_RET_EXITCODE("cfg.getParamAsString");

	return fvaLoadStrListFromFile(rootSWdir + "#data#/FVA_ERROR_NO_EXIF_LOCATION.csv", fileListToFillUp);
}
FVA_EXIT_CODE FVAFlowController::MoveInputDirToOutputDirs(const QString& inputDir, const STR_LIST& outputDirs, bool removeInput, QObject* obj)
{
	// get the size of folder list we received
	uint sizeProcessed = outputDirs.size();

	CLTContext context; // empty so far
	context.dir 	= inputDir;
	context.cmdType	= "CLTMoveInputDir2Output";

	// for each folder in output list
	for (STR_LIST::const_iterator it = outputDirs.begin(); it != outputDirs.end(); ++it)
	{	
		QString dirToMoveTo	= *it;	
		context.outputDir       = dirToMoveTo;
		LOG_DEB << "Moving into:" << dirToMoveTo;

		// remove before rename if destination exists
		// fvaRemoveDirIfEmpty(dirToMoveTo);

		// check if we got 1 folder only to integrate the multimedia data  into 
		// and if we need to remove the input folder as well
		if (1 == sizeProcessed && removeInput)
			// in read only it moves the content
		{
			context.readOnly = true;
			LOG_DEB << "Moving Content";
		} 
		else
		{
			// otherwise it copies content (if readonly is NO)
			context.readOnly = false;
			LOG_DEB << "Copying Content";	 
		}
		context.custom		= "merge";
		FVA_EXIT_CODE exitCode	= m_dataProcessor.run(context, m_cfg);

		if (exitCode == FVA_ERROR_DEST_DIR_ALREADY_EXISTS)
		{
			// ask user for what to do 
			// TODO to make multilanguage
			QMessageBox msgBox;
			msgBox.setText("The sub directory for merge is already present in target!");
			msgBox.setInformativeText("By pressing OK all the content will be merged to already existing folder,\n by pressing Cancel new dir will be created to keep the content");
			msgBox.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
			msgBox.setDefaultButton(QMessageBox::Ok);

			context.custom = (msgBox.exec() == QMessageBox::Ok) ? "merge" : "create";
		}

		// and run it again
		exitCode		= m_dataProcessor.run(context, m_cfg);

		if (FVA_ERROR_DEST_FILE_ALREADY_EXISTS == exitCode)
		{

			CLTContext contextDupl; // empty so far
			contextDupl.dir 	= inputDir;
			contextDupl.cmdType	= "CLTFixDuplicatedFileNames";

			// run new cmd
			exitCode		= m_dataProcessor.run(contextDupl, m_cfg);
			
			// show error message box and return to calling function if previous operation failed
			IF_CLT_ERROR_SHOW_MSG_BOX_AND_RET_EXITCODE(contextDupl.cmdType)
			
			// and run move cmd again
			exitCode		= m_dataProcessor.run(context, m_cfg);
		}
		// show error message box and return to calling function if previous operation failed
		IF_CLT_ERROR_SHOW_MSG_BOX_AND_RET_EXITCODE(context.cmdType)

	}

	QString fvaSWRootDir;
	FVA_EXIT_CODE exitCode = m_cfg.getParamAsString("Common::RootDir", fvaSWRootDir);

	// show error message box and return to calling function if previous operation failed
	IF_CLT_ERROR_SHOW_MSG_BOX_AND_RET_EXITCODE("cfg.getParamAsString");

	QStringList params;
	params.append(fvaSWRootDir + "#data#/fvaFile.csv");
	params.append(fvaSWRootDir + "#data#/fvaFileN.csv");

	// run command implemented in python to the fvafile.csv and fvafileN.csv
	exitCode = runPythonCMD("CLTMerge2csv.py", obj, params);
	
	// show error message box and return to calling function if previous operation failed
	IF_CLT_ERROR_SHOW_MSG_BOX_AND_RET_EXITCODE(context.cmdType)

	// clean up after processing
	QFile::remove(fvaSWRootDir + "#data#/fvaFileN.csv");

	// TODO - call CLTAutoChecks3 for all merged folders

	return FVA_NO_ERROR;
}