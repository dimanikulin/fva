/*!
* \file fvalogger.inl
* \copyright Copyright 2021 FVA Software. All rights reserved. This file is released under the XXX License.
* \author Dima Nikulin.
* \version 0.29
* \date  2014-2021
*/

#ifndef FVALOGGER_H
#define FVALOGGER_H

#include <fstream>
#include <QtCore/QDebug>
#include <QtCore/QDateTime>

#include "fvaconstants.h"

#define LOG_WARN qWarning() <<"[WRN]"<< QDateTime::currentDateTime().toString(LOG_DT_FORMAT).toLatin1().data() << "[" << __FILE__ << "]" << "[" << __LINE__ << "]"
#define LOG_CRIT qCritical()<<"[ERR]"<< QDateTime::currentDateTime().toString(LOG_DT_FORMAT).toLatin1().data() << "[" << __FILE__ << "]" << "[" << __LINE__ << "]"
#define LOG_DEB qDebug()    <<"[DBG]"<< QDateTime::currentDateTime().toString(LOG_DT_FORMAT).toLatin1().data() << "[" << __FILE__ << "]" << "[" << __LINE__ << "]"


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
exitCode = cfg.getParamAsString("Common::RootDir", logPath); \
logPath += + LOG_NAME;\
g_logfile.open(logPath.toStdString(), std::ios::app);\
\
uint dLogLevel = 0; \
exitCode = cfg.getParamAsUint("Common::LogLevel", dLogLevel); \
g_logLevel = (QtMsgType)dLogLevel;


#endif //FVALOGGER_H