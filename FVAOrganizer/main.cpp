/*!
* \file main.cpp
* \copyright Copyright 2021 FVA Software. All rights reserved. This file is released under the XXX License.
* \author Dima Nikulin.
* \version 0.29
* \date  2014-2021
*/
#include <QtCore/QDateTime>
#include <QMessageBox>
#include <QApplication>

#include "FVADataProcessor.h"
#include "fvacommonlib.h"
#include "fvalogger.inl"
#include "fvacommonui.h"

LOGGER_GLOBAL

int main( int argc, char *argv[] )
{
	QApplication a(argc, argv);

	FvaConfiguration cfg;
	FVA_EXIT_CODE exitCode = cfg.load(QCoreApplication::applicationDirPath() + "/fvaParams.csv");
	IF_CLT_ERROR_SHOW_MSG_BOX_AND_RET_EXITCODE("param loading")

	LOG_INIT("#logs#/org.txt")

        LOG_DEB << "FVAOrganizer logger started";

	CLTContext context;
	QString temp;
	if ( a.arguments().size() < 3 || a.arguments().size() > 7 )
	{
		QMessageBox msgBox;
		msgBox.setText("Not enough parameters!\n"\
		"argument [0] - path \n" \
		"argument [1] - cmdType \n" \
		"argument [2] - path to folder or file \n" \
		"argument [3] - recursive \n" \
		"argument [4] - logLevel \n" \
		"argument [5] - readonly \n" \
		"argument [6] - custom \n\n" \
		"example: CLTAutoChecks2 \"C:/FVA/2009/2009.09.05\" recursive=yes logvel=4 readonly=no custom=someValue");
		msgBox.exec();

		return FVA_ERROR_NOT_ENOUGH_ARG;
	}
	switch ( a.arguments().size() )
	{
		case 7:
		{
			context.custom = a.arguments()[6];
			if (context.custom.contains("custom="))
				context.custom = context.custom.remove("custom=");
		}

		case 6:
		{
			temp = a.arguments()[5];
			if (temp.contains("readonly="))
				temp = temp.remove("readonly=");
			context.readOnly = (temp == "yes");
 		}

		case 5:
		{
			// TODO to change to anoter parameter when needs because it is now saved into cfg file - fvaParams.csv
			/*logLevel = a.arguments()[4];
			if ( logLevel.contains("loglevel=") )
			{
				logLevel = logLevel.remove( "loglevel=" );
				int dLogLevel = logLevel.toInt();
				if ( dLogLevel )
					g_logLevel = ( QtMsgType ) dLogLevel;
			}*/ 
		}
		case 4:
		{
			temp = a.arguments()[3];
			if (temp.contains("recursive="))
				temp = temp.remove("recursive=");
			context.readOnly = (temp == "yes");
		}
		case 3:
		{
			context.dir = a.arguments()[2]; 
		}
		case 2:
		{
			context.cmdType = a.arguments()[1];
		}
	}

	FVADataProcessor cmdExecutor;
	int result = cmdExecutor.run(context, cfg);
	g_logfile.close();
	return result;
}