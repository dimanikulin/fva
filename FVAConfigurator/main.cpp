/*!
* \file main.cpp
* \copyright Copyright 2021 FVA Software. All rights reserved. This file is released under the XXX License.
* \author Dima Nikulin.
* \version 0.29
* \date  2014-2021
*/

#include "fvacommonui.h"
#include "fvalogger.inl"
#include "fvaconfigurator.h"
#include <QtWidgets/QApplication>

LOGGER_GLOBAL

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	FvaConfiguration cfg;
	FVA_EXIT_CODE exitCode = cfg.load(QCoreApplication::applicationDirPath() + "/fvaParams.csv");
	IF_CLT_ERROR_SHOW_MSG_BOX_AND_RET_EXITCODE("param loading")

	LOG_INIT("#logs#/configurator.txt")
	LOG_DEB << "Configurator logger started";

	FVAConfigurator w;
	w.show();
	return a.exec();
}
