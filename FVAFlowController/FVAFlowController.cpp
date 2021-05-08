#include "FVAFlowController.h"

#include <QtCore/QProcess>
#include <QtCore/QCoreApplication>
#include <QtCore/QDir>

#include "FVAConfiguration.h"
#include "fvacommonui.h"
#include "fvacommoncsv.h"

FVAFlowController::FVAFlowController()
{
	FVA_EXIT_CODE exitCode = m_cfg.load(QCoreApplication::applicationDirPath() + "/fvaParams.csv");
	IF_CLT_ERROR_SHOW_MSG_BOX_AND_RET("cfg.load")
}

FVA_EXIT_CODE FVAFlowController::performDeviceChecks(DeviceContext& deviceContext, CLTContext& context, const FvaConfiguration& cfg)
{
	QString rootSWdir;
	FVA_EXIT_CODE exitCode = cfg.getParamAsString("Common::RootDir", rootSWdir);
	IF_ERROR_SHOW_MSG_BOX_AND_RET_EXITCODE("getParamAsString(Common::RootDir)")

	context.cmdType = "CLTCheckDeviceName";
	exitCode = m_dataProcessor.run(context, cfg);
	if (FVA_ERROR_NON_UNIQUE_DEVICE_NAME == exitCode)
	{
		context.cmdType = "CLTCreateDirStructByDeviceName";
		exitCode = m_dataProcessor.run(context, cfg);
		FVA_MESSAGE_BOX("Found several devices in a folder, please select other dir!");
		return exitCode;
	}
	else IF_CLT_ERROR_SHOW_MSG_BOX_AND_RET_EXITCODE("CLTCheckDeviceName")

	exitCode = fvaLoadDeviceMapFromCsv(rootSWdir, deviceContext.fullDeviceMap);
	IF_ERROR_SHOW_MSG_BOX_AND_RET_EXITCODE("fvaLoadDeviceMapFromCsv")

	PEOPLE_MAP peopleMap;
	exitCode = fvaLoadPeopleMapFromCsv(rootSWdir, peopleMap);
	IF_ERROR_SHOW_MSG_BOX_AND_RET_EXITCODE("fvaLoadPeopleMapFromCsv")

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
	IF_CLT_ERROR_SHOW_MSG_BOX_AND_RET_EXITCODE("CLTCheckFileFormat")

	context.cmdType = "CLTRenameVideoBySequence";
	exitCode = m_dataProcessor.run(context, cfg);
	IF_CLT_ERROR_SHOW_MSG_BOX_AND_RET_EXITCODE("CLTRenameVideoBySequence")

	context.cmdType = "CLTConvertAmr";
	exitCode = m_dataProcessor.run(context, cfg);
	IF_CLT_ERROR_SHOW_MSG_BOX_AND_RET_EXITCODE("CLTConvertAmr")

	context.cmdType = "CLTAutoChecks1";
	exitCode = m_dataProcessor.run(context, cfg);
	IF_CLT_ERROR_SHOW_MSG_BOX_AND_RET_EXITCODE("CLTAutoChecks1")

	context.cmdType = "CLTSetFileAtts";
	IF_CLT_ERROR_SHOW_MSG_BOX_AND_RET_EXITCODE("CLTSetFileAtts")

	return FVA_NO_ERROR;
}

FVA_EXIT_CODE FVAFlowController::PerformChecksForInputDir(const QString& dir, DeviceContext& deviceContext, QObject* obj)
{
	CLTContext context;
	context.dir = dir;
	FVA_EXIT_CODE res = performCommonChecks(context, m_cfg);
	RET_RES_IF_RES_IS_ERROR

	// do we need to search by device?
	bool SearchByDevice = false;
	FVA_EXIT_CODE exitCode = m_cfg.getParamAsBoolean("Search::Device", SearchByDevice);
	IF_ERROR_SHOW_MSG_BOX_AND_RET_EXITCODE("getParamAsBoolean(Search::Device)")
	if (SearchByDevice)
	{
		FVA_EXIT_CODE res = performDeviceChecks(deviceContext, context, m_cfg);
		RET_RES_IF_RES_IS_ERROR
	}

	// do we need to search by date-time?
	bool SearchByDateTime = false;
	exitCode = m_cfg.getParamAsBoolean("Search::DateTime", SearchByDateTime);
	IF_ERROR_SHOW_MSG_BOX_AND_RET_EXITCODE("getParamAsBoolean(Search::DateTime)")
	if (SearchByDateTime)
	{
		FVA_EXIT_CODE res = performDTChecks(context, m_cfg);
		RET_RES_IF_RES_IS_ERROR
	}

	// do we need to search by location?
	bool SearchByLocation = false;
	exitCode = m_cfg.getParamAsBoolean("Search::Location", SearchByLocation);
	IF_ERROR_SHOW_MSG_BOX_AND_RET_EXITCODE("getParamAsBoolean(Search::Location)")
	if (SearchByLocation)
	{
		FVA_EXIT_CODE res = performLocationChecks(context, m_cfg);
		RET_RES_IF_RES_IS_ERROR
	}

	bool needCheckOrientation = false;
	exitCode = m_cfg.getParamAsBoolean("Common::CheckOrientation", needCheckOrientation);
	IF_ERROR_SHOW_MSG_BOX_AND_RET_EXITCODE("getParamAsBoolean with Common::CheckOrientation")

	if (needCheckOrientation)
		performOrientationChecks(dir,obj);

	return FVA_NO_ERROR;
}
FVA_EXIT_CODE FVAFlowController::performDTChecks(CLTContext& context, const FvaConfiguration& cfg)
{
	context.cmdType = "CLTCheckDateTime";
	FVA_EXIT_CODE exitCode = m_dataProcessor.run(context, cfg);
	IF_CLT_ERROR_SHOW_MSG_BOX_AND_RET_EXITCODE("CLTCheckDateTime")
	return FVA_NO_ERROR;
}

FVA_EXIT_CODE FVAFlowController::performLocationChecks(CLTContext& context, const FvaConfiguration& cfg)
{
	context.cmdType = "CLTCheckLocation";
	FVA_EXIT_CODE exitCode = m_dataProcessor.run(context, cfg);
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
	IF_CLT_ERROR_SHOW_MSG_BOX_AND_RET_EXITCODE("CLTRenameFiles RO mode")

	context.readOnly = false;
	exitCode = m_dataProcessor.run(context, m_cfg);
	IF_CLT_ERROR_SHOW_MSG_BOX_AND_RET_EXITCODE("CLTRenameFiles")

	context.cmdType = "CLTCSVFvaFile";
	context.custom = QString::number(deviceId);
	exitCode = m_dataProcessor.run(context, m_cfg);
	IF_CLT_ERROR_SHOW_MSG_BOX_AND_RET_EXITCODE("CLTCSVFvaFile")
	context.custom = "";

	context.cmdType = "CLTCreateDirStructByFileNames";
	exitCode = m_dataProcessor.run(context, m_cfg);
	IF_CLT_ERROR_SHOW_MSG_BOX_AND_RET_EXITCODE("CLTCreateDirStructByFileNames")

	context.cmdType = "CLTMoveAloneFiles";
	exitCode = m_dataProcessor.run(context, m_cfg);
	IF_CLT_ERROR_SHOW_MSG_BOX_AND_RET_EXITCODE("CLTMoveAloneFiles")

	context.cmdType = "CLTGetFvaDirType";
	context.recursive = false;
	exitCode = m_dataProcessor.run(context, m_cfg);
	context.recursive = true;

	context.cmdType = "CLTAutoChecks2";
	exitCode = m_dataProcessor.run(context, m_cfg);
	IF_CLT_ERROR_SHOW_MSG_BOX_AND_RET_EXITCODE("CLTAutoChecks2")

	return FVA_NO_ERROR;
}

FVA_EXIT_CODE FVAFlowController::ProcessInputDirForEventCfg(const QString& inputDir, const QString& outputDir)
{
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
		IF_CLT_ERROR_SHOW_MSG_BOX_AND_RET_FALSE("CLTMerge1DayEventDir")

		if (FVA_ERROR_DEST_FILE_ALREADY_EXISTS == exitCode)
		{
			exitCode = fvaRunCLT("CLTFixDuplicatedFileNames", ((FVAOrganizerWizard*)wizard())->inputFolder());
			IF_CLT_ERROR_SHOW_MSG_BOX_AND_RET_FALSE("CLTFixDuplicatedFileNames")
				exitCode = fvaRunCLT("CLTMerge1DayEventDir", ((FVAOrganizerWizard*)wizard())->inputFolder());
		}
		IF_CLT_ERROR_SHOW_MSG_BOX_AND_RET_FALSE("CLTMerge1DayEventDir")
	}
	else if (oneEventSeveralDays->isChecked())
	{
		exitCode = fvaRunCLT("CLTMerge1EventDir", ((FVAOrganizerWizard*)wizard())->inputFolder(), true, false, outputDirLineEdit->text());

		if (FVA_ERROR_DEST_FILE_ALREADY_EXISTS == exitCode)
		{
			exitCode = fvaRunCLT("CLTFixDuplicatedFileNames", ((FVAOrganizerWizard*)wizard())->inputFolder());
			IF_CLT_ERROR_SHOW_MSG_BOX_AND_RET_FALSE("CLTFixDuplicatedFileNames")
				exitCode = fvaRunCLT("CLTMerge1EventDir", ((FVAOrganizerWizard*)wizard())->inputFolder());
		}
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
		IF_CLT_ERROR_SHOW_MSG_BOX_AND_RET_FALSE("CLTAutoChecks3")
	}
	else
	{
		// TODO - apply call CLTAutoChecks3 for all merged folders
	}*/

	return FVA_NO_ERROR;
}
FVA_EXIT_CODE FVAFlowController::MoveInputDirToOutputDir(const QString& inputDir, const STR_LIST& outputDirs, bool removeInput)
{
	
	return FVA_NO_ERROR;
}