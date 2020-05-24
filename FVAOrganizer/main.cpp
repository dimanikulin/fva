#include <QtCore/QCoreApplication>
#include <QDateTime>
#include <QDebug>

#include "cmdlineexecutor.h"
#include "fvacommonlib.h"

#include <fstream>

/*!
 * global log file
 */
std::ofstream	g_logfile;

/*!
 * global log file
 */
QtMsgType	g_logLevel = QtDebugMsg;

void msghandler( QtMsgType type, const char *msg )
{
	if ( type < g_logLevel )
		return;
	
	g_logfile << QDateTime::currentDateTime().toString( "[yyyy:MM:dd-hh:mm:ss]").toAscii().data();
	switch ( type ) 
	{
		case QtDebugMsg:
			g_logfile << "[DBG]" << msg << "\n";
		break;
		case QtWarningMsg:
			g_logfile << "[WRN]" << msg << "\n";
		break;
		case QtCriticalMsg:
			g_logfile << "[ERR]" << msg << "\n";
		break;
		case QtFatalMsg:
		{
			g_logfile << "[FAT]" << msg << "\n";
			abort();
		}
	}	
}

int main( int argc, char *argv[] )
{	
	//install : set the callback
	// TODO make output configurable
	// qInstallMsgHandler( msghandler );

	QCoreApplication a(argc, argv);

	QString logPath = QCoreApplication::applicationDirPath() + "\\organizerlog.txt";  
	g_logfile.open( logPath.toStdString(), std::ios::app );
	QString cmdType, path, recursive, logLevel, readOnly, custom;
	if ( a.arguments().size() < 3 || a.arguments().size() > 7 )
	{
		// TODO print usage
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