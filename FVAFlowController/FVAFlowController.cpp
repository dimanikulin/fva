#include "FVAFlowController.h"

#include <QtCore/QProcess>
#include <QtCore/QCoreApplication>
#include <QtCore/QDir>

#include "FVAConfiguration.h"
#include "fvacommonlib.h"
#include "fvacommonui.h"
#include "fvacommoncsv.h"
#include "fvaconstants.h"

#include "fvadataprocessor.h"

FVA_EXIT_CODE FVAFlowController::performDeviceChecks(DeviceContext& deviceContext, CLTContext& context, const FvaConfiguration& cfg)
{
	QString rootSWdir;
	FVA_EXIT_CODE exitCode = cfg.getParamAsString("Common::RootDir", rootSWdir);
	IF_ERROR_SHOW_MSG_BOX_AND_RET_EXITCODE("getParamAsString(Common::RootDir)")

	context.cmdType = "CLTCheckDeviceName";
	exitCode = FVADataProcessor::run(context, cfg);
	if (FVA_ERROR_NON_UNIQUE_DEVICE_NAME == exitCode)
	{
		context.cmdType = "CLTCreateDirStructByDeviceName";
		exitCode = FVADataProcessor::run(context, cfg);
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
	FVA_EXIT_CODE exitCode = FVADataProcessor::run(context, cfg);
	IF_CLT_ERROR_SHOW_MSG_BOX_AND_RET_EXITCODE("CLTCheckFileFormat")

	context.cmdType = "CLTRenameVideoBySequence";
	exitCode = FVADataProcessor::run(context, cfg);
	IF_CLT_ERROR_SHOW_MSG_BOX_AND_RET_EXITCODE("CLTRenameVideoBySequence")

	context.cmdType = "CLTConvertAmr";
	exitCode = FVADataProcessor::run(context, cfg);
	IF_CLT_ERROR_SHOW_MSG_BOX_AND_RET_EXITCODE("CLTConvertAmr")

	context.cmdType = "CLTAutoChecks1";
	exitCode = FVADataProcessor::run(context, cfg);
	IF_CLT_ERROR_SHOW_MSG_BOX_AND_RET_EXITCODE("CLTAutoChecks1")

	return FVA_NO_ERROR;
}

FVA_EXIT_CODE FVAFlowController::PerformChecksForInputDir(const QString& dir, DeviceContext& deviceContext, QObject* obj)
{
	FvaConfiguration cfg;
	FVA_EXIT_CODE exitCode = cfg.load(QCoreApplication::applicationDirPath() + "/fvaParams.csv");
	IF_ERROR_SHOW_MSG_BOX_AND_RET_EXITCODE("cfg.load")

	CLTContext context;
	context.dir = dir;
	FVA_EXIT_CODE res = performCommonChecks(context, cfg);
	RET_RES_IF_RES_IS_ERROR

	// do we need to search by device?
	bool SearchByDevice = false;
	exitCode = cfg.getParamAsBoolean("Search::Device", SearchByDevice);
	IF_ERROR_SHOW_MSG_BOX_AND_RET_EXITCODE("getParamAsBoolean(Search::Device)")
	if (SearchByDevice)
	{
		FVA_EXIT_CODE res = performDeviceChecks(deviceContext, context, cfg);
		RET_RES_IF_RES_IS_ERROR
	}

	// do we need to search by date-time?
	bool SearchByDateTime = false;
	exitCode = cfg.getParamAsBoolean("Search::DateTime", SearchByDateTime);
	IF_ERROR_SHOW_MSG_BOX_AND_RET_EXITCODE("getParamAsBoolean(Search::DateTime)")
	if (SearchByDateTime)
	{
		FVA_EXIT_CODE res = performDTChecks(context, cfg);
		RET_RES_IF_RES_IS_ERROR
	}

	// do we need to search by location?
	bool SearchByLocation = false;
	exitCode = cfg.getParamAsBoolean("Search::Location", SearchByLocation);
	IF_ERROR_SHOW_MSG_BOX_AND_RET_EXITCODE("getParamAsBoolean(Search::Location)")
	if (SearchByLocation)
	{
		FVA_EXIT_CODE res = performLocationChecks(context, cfg);
		RET_RES_IF_RES_IS_ERROR
	}

	bool needCheckOrientation = false;
	exitCode = cfg.getParamAsBoolean("Common::CheckOrientation", needCheckOrientation);
	IF_ERROR_SHOW_MSG_BOX_AND_RET_EXITCODE("getParamAsBoolean with Common::CheckOrientation")

	if (needCheckOrientation)
		performOrientationChecks(dir,obj);

	return FVA_NO_ERROR;
}
FVA_EXIT_CODE FVAFlowController::performDTChecks(CLTContext& context, const FvaConfiguration& cfg)
{
	context.cmdType = "CLTCheckDateTime";
	FVA_EXIT_CODE exitCode = FVADataProcessor::run(context, cfg);
	IF_CLT_ERROR_SHOW_MSG_BOX_AND_RET_EXITCODE("CLTCheckDateTime")
	return FVA_NO_ERROR;
}

FVA_EXIT_CODE FVAFlowController::performLocationChecks(CLTContext& context, const FvaConfiguration& cfg)
{
	context.cmdType = "CLTCheckLocation";
	FVA_EXIT_CODE exitCode = FVADataProcessor::run(context, cfg);
	IF_CLT_ERROR_SHOW_MSG_BOX_AND_RET_EXITCODE("CLTCheckLocation")
	return FVA_NO_ERROR;
}