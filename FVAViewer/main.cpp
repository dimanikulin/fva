#include "fvaviewer.h"
#include <QtGui/QApplication>
#include "cmdlineexecutor.h"

#include <fstream>

using namespace std;

/*!
 * global log file
 */
ofstream	g_logfile;

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
	qInstallMsgHandler( msghandler ); 

	QApplication a(argc, argv);

	QString logPath = QApplication::applicationDirPath() + "\\logfile.txt";  
	g_logfile.open( logPath.toStdString(), ios::app );
	int result = 0;
	
	// means usual view mode
	if ( 1 == argc )
	{
		FVAViewer w;
		w.show();
		result = a.exec();
		g_logfile.close();
		return result;
	}
	else
	{ // command line processing assumed
		
		if ( a.arguments().size() >=4 )
		{
			QString logLevel = a.arguments()[3];
			if ( logLevel.contains("loglevel=") )
			{
				logLevel = logLevel.remove( "loglevel=" );
				int dLogLevel = logLevel.toInt();
				if ( dLogLevel )
					g_logLevel = ( QtMsgType ) dLogLevel;
			}
		}
		qWarning() << "[MAIN] command started!";
		CmdLineExecutor cmdExecutor( a.arguments() );
		result = cmdExecutor.run();
		qWarning() << "[MAIN] command finished!";
		g_logfile.close();
		return result;
	}
}