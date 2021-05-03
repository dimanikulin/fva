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
	FVA_EXIT_CODE exitCode = fvaRunCLT("CLTCheckDeviceName", dir);
	if (FVA_ERROR_NON_UNIQUE_DEVICE_NAME == exitCode)
	{
		exitCode = fvaRunCLT("CLTCreateDirStructByDeviceName", dir);
		FVA_MESSAGE_BOX("Found several devices in a folder, please select other dir!");
		return exitCode;
	}
	else IF_CLT_ERROR_SHOW_MSG_BOX_AND_RET_FALSE("CLTCheckDeviceName")

	FvaConfiguration cfg;
	exitCode = cfg.load(QCoreApplication::applicationDirPath() + "/fvaParams.csv");
	if (FVA_NO_ERROR != exitCode)
	{
		FVA_MESSAGE_BOX("cfg.load failed with error " + QString::number(exitCode));
		return exitCode;
	}

	QString rootSWdir;
	exitCode = cfg.getParamAsString("Common::RootDir", rootSWdir);
	if (FVA_NO_ERROR != exitCode)
		return exitCode;

	exitCode = fvaLoadDeviceMapFromCsv(rootSWdir, deviceContext.fullDeviceMap);
	if (FVA_NO_ERROR != exitCode)
	{
		FVA_MESSAGE_BOX("fvaLoadDeviceMapFromCsv failed with error " + QString::number(exitCode));
		return exitCode;
	}

	PEOPLE_MAP peopleMap;
	exitCode = fvaLoadPeopleMapFromCsv(rootSWdir, peopleMap);
	if (FVA_NO_ERROR != exitCode)
	{
		FVA_MESSAGE_BOX("fvaLoadPeopleMapFromCsv failed with error " + QString::number(exitCode));
		return exitCode;
	}

	for (auto it = deviceContext.fullDeviceMap.begin(); it != deviceContext.fullDeviceMap.end(); ++it)
	{
		it.value().ownerName = peopleMap[it.value().ownerId].name;
	}

	QDir _dir(dir);
	Q_FOREACH(QFileInfo info, _dir.entryInfoList(QDir::System | QDir::Hidden | QDir::Files, QDir::DirsLast))
	{
		if (info.isDir())
			continue;
		QString suffix = info.suffix().toUpper();
		FVA_FS_TYPE type = fvaConvertFileExt2FileType(suffix);
		if (FVA_FS_TYPE_IMG != type)
			continue;

		QString deviceName;
		deviceContext.deviceMap = fvaGetDeviceMapForImg(deviceContext.fullDeviceMap, info.filePath(), deviceName);

		if (deviceName.isEmpty())
			continue;
		else
		{
			deviceContext.matchedDeviceName = deviceName;
			break;
		}
	}

	bool needCheckOrientation;
	exitCode = cfg.getParamAsBoolean("Common::CheckOrientation", needCheckOrientation);
	if (FVA_NO_ERROR != exitCode)
	{
		FVA_MESSAGE_BOX("getParamAsBoolean with Common::CheckOrientation " + QString::number(exitCode));
		return exitCode;
	}

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