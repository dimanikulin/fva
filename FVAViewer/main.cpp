#include "fvaviewer.h"
#include <QtGui/QApplication>

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

int main(int argc, char *argv[])
{
	//install : set the callback
	// qInstallMsgHandler( msghandler ); 

	QApplication a(argc, argv);
	QTextCodec::setCodecForTr(QTextCodec::codecForName("Windows-1251"));
	QString logPath = QApplication::applicationDirPath() + "\\viewerlog.txt";  
	g_logfile.open( logPath.toStdString(), std::ios::app );
	 
	QString rootDir (( a.arguments().size() >= 2 ) ? a.arguments() [1] : QApplication::applicationDirPath());
	QString dictPath (( a.arguments().size() >= 3 ) ? a.arguments() [2] : QApplication::applicationDirPath() + "/" + FVA_DICTIONARY_NAME );

	FVAViewer w( rootDir, dictPath );
	w.show();
	int result = a.exec();
	g_logfile.close();
	return result;
}
