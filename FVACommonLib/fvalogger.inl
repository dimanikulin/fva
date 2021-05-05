#ifndef FVALOGGER_H
#define FVALOGGER_H

#include <fstream>
#include <QtCore/QDebug>

#define LOGGER_GLOBAL\
/*! \
* global logger \
*/\
std::ofstream g_logfile;\
\
/*! \
* global log level\
*/\
QtMsgType	g_logLevel = QtDebugMsg; \
\
void msghandler(QtMsgType type, const QMessageLogContext &, const QString & str)\
{\
	if (type < g_logLevel)\
		return;\
	g_logfile << str.toLatin1().data() << "\n"; \
	g_logfile.flush();\
}

#define LOG_INIT(LOG_NAME)\
qInstallMessageHandler(msghandler); \
QString logPath; \
res = cfg.getParamAsString("Common::RootDir", logPath); \
logPath += "#logs/#LOG_NAME";\
g_logfile.open(logPath.toStdString(), std::ios::app);\
\
uint dLogLevel = 0; \
res = cfg.getParamAsUint("Common::LogLevel", dLogLevel); \
g_logLevel = (QtMsgType)dLogLevel;


#endif //FVALOGGER_H