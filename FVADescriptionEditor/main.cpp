/*!
* \file main.cpp
* \copyright Copyright 2021 FVA Software. All rights reserved. This file is released under the XXX License.
* \author Dima Nikulin.
* \version 0.29
* \date  2014-2021
*/
#include "fvadescriptioneditor.h"
#include "fvadescriptionfile.h"
#include "fvacommonlib.h"

#include <QtWidgets/QApplication>
#include <QtCore/QDir>
#include <QtWidgets/QMessageBox>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	FvaConfiguration cfg;
	FVA_EXIT_CODE res = cfg.load(QCoreApplication::applicationDirPath() + "/fvaParams.csv");
	IF_ERROR_SHOW_MSG_BOX_AND_RET_EXITCODE("param loading")

	LOG_INIT("#logs#/desceditor.txt")

	bool folderMode = true;
	FVADescriptionEditor w( folderMode, nullptr);

	return a.exec();
}