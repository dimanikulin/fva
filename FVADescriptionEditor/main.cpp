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
	
	bool folderMode = true;

	FVADescriptionEditor w( folderMode, nullptr);
	w.show();
	return a.exec();
}