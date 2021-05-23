/*!
* \file main.cpp
* \copyright Copyright 2021 FVA Software. All rights reserved. This file is released under the XXX License.
* \author Dima Nikulin.
* \version 0.29
* \date  2014-2021
*/
#include "fvadictionaryeditor.h"
#include <QtWidgets/QApplication>
#include "fvacommonlib.h"
#include <QtCore/qtextcodec>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	
	// get application input parameters into list
	QStringList list;
	for (int a = 0; a < argc; ++a) {
		list << QString::fromLocal8Bit(argv[a]);
	}

	if (list.size() < 2)
	{
		return FVA_ERROR_NOT_ENOUGH_ARG;
	}
	QString deviceMatchName;
	if (list.size() >= 3)
	{
		deviceMatchName = list.at(2);
	}
	FVADictionaryEditor w(deviceMatchName);
	w.show();
	return a.exec();
}
