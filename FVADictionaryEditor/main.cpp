/*!
* \file main.cpp
* \copyright Copyright 2021 FVA Software. All rights reserved. This file is released under the XXX License.
* \author Dima Nikulin.
* \version 0.29
* \date  2014-2021
*/
#include "fvadictionaryeditor.h"

#include "fvacommonlib.h"
#include "fvacommonui.h"
#include "fvalogger.inl"

#include <QApplication>
#include <QtCore/qtextcodec>

LOGGER_GLOBAL

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);

	FvaConfiguration cfg;
	FVA_EXIT_CODE exitCode = cfg.load(QCoreApplication::applicationDirPath() + "/fvaParams.csv");
	IF_CLT_ERROR_SHOW_MSG_BOX_AND_RET_EXITCODE("param loading")

	LOG_INIT("#logs#/dicteditor.txt")
        LOG_DEB << "Dictionary editor logger started";
	
	// get application input parameters and transform into list
	QStringList list;
	for (int a = 0; a < argc; ++a) {
		list << QString::fromLocal8Bit(argv[a]);
		LOG_DEB << "Dictionary editor input param [" << a << "] = " << QString::fromLocal8Bit(argv[a]);
	}

	QString deviceMatchName;
	if (list.size() >= 2)
	{
		deviceMatchName = list.at(1);
	}

	FVADictionaryEditor w(deviceMatchName);
	w.show();
	return a.exec();
}
