#include "FVAFlowController.h"

#include <QtCore/QProcess>
#include <QtCore/QCoreApplication>
#include <QtCore/QDir>

#include "FVAConfiguration.h"
#include "fvacommonlib.h"
#include "fvacommonui.h"
#include "fvacommoncsv.h"
#include "fvaconstants.h"

FVA_EXIT_CODE FVAFlowController::PerformChecksForInputDir(const QString& dir, DeviceContext& deviceContext, QObject* obj)
{
	FvaConfiguration cfg;
	FVA_EXIT_CODE exitCode = cfg.load(QCoreApplication::applicationDirPath() + "/fvaParams.csv");
	IF_ERROR_SHOW_MSG_BOX_AND_RET_EXITCODE("cfg.load")

	// do we need to search by device?
	bool SearchByDevice = false;
	exitCode = cfg.getParamAsBoolean("Search::Device", SearchByDevice);
	IF_ERROR_SHOW_MSG_BOX_AND_RET_EXITCODE("getParamAsBoolean(Search::Device)")

	QString rootSWdir;
	exitCode = cfg.getParamAsString("Common::RootDir", rootSWdir);
	IF_ERROR_SHOW_MSG_BOX_AND_RET_EXITCODE("getParamAsString(Common::RootDir)")

	if (SearchByDevice)
	{
		exitCode = fvaRunCLT("CLTCheckDeviceName", dir);
		if (FVA_ERROR_NON_UNIQUE_DEVICE_NAME == exitCode)
		{
			exitCode = fvaRunCLT("CLTCreateDirStructByDeviceName", dir);
			FVA_MESSAGE_BOX("Found several devices in a folder, please select other dir!");
			return exitCode;
		}
		else IF_CLT_ERROR_SHOW_MSG_BOX_AND_RET_FALSE("CLTCheckDeviceName")

		exitCode = fvaLoadDeviceMapFromCsv(rootSWdir, deviceContext.fullDeviceMap);
		IF_ERROR_SHOW_MSG_BOX_AND_RET_EXITCODE("fvaLoadDeviceMapFromCsv")

		PEOPLE_MAP peopleMap;
		exitCode = fvaLoadPeopleMapFromCsv(rootSWdir, peopleMap);
		IF_ERROR_SHOW_MSG_BOX_AND_RET_EXITCODE("fvaLoadPeopleMapFromCsv")

		for (auto it = deviceContext.fullDeviceMap.begin(); it != deviceContext.fullDeviceMap.end(); ++it)
			it.value().ownerName = peopleMap[it.value().ownerId].name;

		QDir _dir(dir);
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
	} // if (SearchByDevice)

	bool needCheckOrientation;
	exitCode = cfg.getParamAsBoolean("Common::CheckOrientation", needCheckOrientation);
	IF_ERROR_SHOW_MSG_BOX_AND_RET_EXITCODE("getParamAsBoolean with Common::CheckOrientation")

	if (needCheckOrientation)
	{
		// to run change orentation in auto mode
		QProcess myProcess(obj);
		myProcess.setProcessChannelMode(QProcess::MergedChannels);
		myProcess.start(QCoreApplication::applicationDirPath() + "/jpegr/jpegr.exe -auto " + dir);
		myProcess.waitForFinished(-1);
	}
	return FVA_NO_ERROR;
}