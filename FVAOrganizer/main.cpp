#include <QtCore/QCoreApplication>
#include <QtCore/QDateTime>
#include <QtCore/QDebug>

#include "cmdlineexecutor.h"
#include "fvacommonlib.h"
#include "fvadefaultcfg.h"

#include <fstream>

/*!
 * global log file
 */
std::ofstream	g_logfile;

/*!
 * global log file
 */
QtMsgType	g_logLevel = QtDebugMsg;

void msghandler(QtMsgType type, const QMessageLogContext &, const QString & str)
{
	const char * msg = str.toStdString().c_str();
	if ( type < g_logLevel )
		return;	
	g_logfile << str.toLatin1().data() << "\n";
}

int main( int argc, char *argv[] )
{
	//install : set the callback
	qInstallMessageHandler(msghandler);

	QString logPath = FVA_DEFAULT_ROOT_DIR + "organizerlog.txt";  
	g_logfile.open( logPath.toStdString(), std::ios::app );

	QCoreApplication a(argc, argv);

	QString cmdType, path, recursive, logLevel, readOnly, custom;
	if ( a.arguments().size() < 3 || a.arguments().size() > 7 )
	{
		// argList[0] /*path*/
		// argList[1] /*cmdType*/, 
		// argList[2] /*path to folder or file*/,
		// argList[3] /*recursive*/,
		// argList[4] /*logLevel*/,
		// argList[5] /*readonly*/,
		// argList[6] /*custom*/,

		qCritical() << "[MAIN] not enough parameters!";
		return FVA_ERROR_NOT_ENOUGH_ARG;
	}
	switch ( a.arguments().size() )
	{
		case 7:
		{
			custom = a.arguments()[6];
			if ( custom.contains("custom=") )
				custom = custom.remove( "custom=" );
		}

		case 6:
		{
			readOnly = a.arguments()[5];
			if ( readOnly.contains("readonly=") )
				readOnly = readOnly.remove( "readonly=" );
		}

		case 5:
		{
			logLevel = a.arguments()[4];
			if ( logLevel.contains("loglevel=") )
			{
				logLevel = logLevel.remove( "loglevel=" );
				int dLogLevel = logLevel.toInt();
				if ( dLogLevel )
					g_logLevel = ( QtMsgType ) dLogLevel;
			}
		}
		case 4:
		{
			recursive = a.arguments()[3];
			if ( recursive.contains("recursive=") )
				recursive = recursive.remove("recursive=");	
		}
		case 3:
		{
			path = a.arguments()[2];
		}
		case 2:
		{
			cmdType = a.arguments()[1];
		}
	}
	CmdLineExecutor cmdExecutor( cmdType, path, recursive == "yes", readOnly=="yes", custom );
	int result = cmdExecutor.run();
	g_logfile.close();
	return result;
}