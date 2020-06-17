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

/*00. to suggest user to run /FVA/#BIN#/jpegr_portable32/jpegr.exe  
#01.RenamedVideoInSequince
#01.1.ConvertedAMR
#01.2.Converted3GP
#02.AutoChecks1
	#01.NotVideoFirst
	#02.NotSTFiles
	#03.NotEmptyDeviceName	
	#04.UniqueDeviceName
#03.RenamedFilesByDate
#04.AddedFolderDescription
#05.CreatedFolderStructure
#06.AutoChecks2
	+#01.ProperFileName
	+#02.MatchingFileNameAndFolder
	+#03.NoUnknownFileTypes
	+#04.NoFileAlone
	+#05.checkedNoFOLDERCopyInside
	+#06.ProperFolderName
	#07.FolderDecriptionValid
	#08.decsriptionFileValid	
#07.folder merging
#08.SetReadOnly*/

	//install : set the callback
	qInstallMsgHandler( msghandler );

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