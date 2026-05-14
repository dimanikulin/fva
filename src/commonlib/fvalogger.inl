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
#include <chrono>
#include <iomanip>
#include <sstream>

extern std::ofstream g_logfile;
extern int g_logLevel;

inline std::string getCurrentTimestamp() {
    auto now = std::chrono::system_clock::now();
    auto time = std::chrono::system_clock::to_time_t(now);
    std::ostringstream oss;
    oss << std::put_time(std::localtime(&time), "[%Y:%m:%d] [%H:%M:%S]");
    return oss.str();
}

#define LOG_WARN if (0 >= g_logLevel) {} else g_logfile << "[WRN]" << getCurrentTimestamp() << " [" << __FILE__ << "] [" << __LINE__ << "] "
#define LOG_CRIT if (1 >= g_logLevel) {} else g_logfile << "[ERR]" << getCurrentTimestamp() << " [" << __FILE__ << "] [" << __LINE__ << "] "
#define LOG_DEB if (2 >= g_logLevel) {} else g_logfile << "[DBG]" << getCurrentTimestamp() << " [" << __FILE__ << "] [" << __LINE__ << "] "

/*!
* global logger 
*/
#define LOGGER_GLOBAL \
std::ofstream g_logfile;

/*!
* global log level (0=warning, 1=critical, 2=debug) \
*/
int g_logLevel = 2;

#define LOG_INIT(LOG_NAME) \
std::string logPath; \
exitCode = cfg.getParamAsString("Common::RootDir", logPath); \
logPath += LOG_NAME; \
g_logfile.open(logPath, std::ios::app); \
unsigned int dLogLevel = 0; \
exitCode = cfg.getParamAsUint("Common::LogLevel", dLogLevel); \
g_logLevel = static_cast<int>(dLogLevel);


#endif //FVALOGGER_H
