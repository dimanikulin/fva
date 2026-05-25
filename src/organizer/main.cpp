/*!
 * \file main.cpp
 * \copyright Copyright 2021 FVA Software. All rights reserved. This file is released under the XXX License.
 * \author Dima Nikulin.
 * \version 0.29
 * \date  2014-2021
 */
#include <cstring>
#include <filesystem>
#include <iostream>
#include <string>

#include "FVADataProcessor.h"
#include "fvacommonlib.h"
#include "fvalogger.inl"

LOGGER_GLOBAL

// TODO to move to commonlib
namespace {
std::string stripPrefix(const std::string& value, const std::string& prefix) {
    if (value.rfind(prefix, 0) == 0) {
        return value.substr(prefix.size());
    }
    return value;
}
}  // namespace

int main(int argc, char* argv[]) {
    std::filesystem::path appDir = std::filesystem::current_path();
    if (argc > 0 && argv[0] != nullptr && std::strlen(argv[0]) > 0) {
        std::filesystem::path appPath(argv[0]);
        if (appPath.has_parent_path()) {
            appDir = appPath.parent_path();
        }
    }

    FvaConfiguration cfg;
    const std::string cfgPath = (appDir / "fvaParams.csv").string();
    FVA_EXIT_CODE exitCode = cfg.load(cfgPath);
    if (exitCode != FVA_NO_ERROR) {
        return exitCode;
    }

    LOG_INIT("#logs#/org.txt")

    LOG_DEB << "FVAOrganizer logger started";

    CLTContext context;
    std::string temp;
    if (argc < 3 || argc > 7) {
        std::cerr
            << "Not enough parameters!\n"
               "argument [0] - path \n"
               "argument [1] - cmdType \n"
               "argument [2] - path to folder or file \n"
               "argument [3] - recursive \n"
               "argument [4] - logLevel \n"
               "argument [5] - readonly \n"
               "argument [6] - custom \n\n"
               "example: CLTAutoChecks2 \"C:/FVA/2009/2009.09.05\" recursive=yes logvel=4 readonly=no custom=someValue"
            << std::endl;

        return FVA_ERROR_NOT_ENOUGH_ARG;
    }
    switch (argc) {
        case 7: {
            context.custom = stripPrefix(argv[6], "custom=");
        }
            [[fallthrough]];

        case 6: {
            temp = stripPrefix(argv[5], "readonly=");
            context.readOnly = (temp == "yes");
        }
            [[fallthrough]];

        case 5: {
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
            [[fallthrough]];
        case 4: {
            temp = stripPrefix(argv[3], "recursive=");
            context.recursive = (temp == "yes");
        }
            [[fallthrough]];
        case 3: {
            context.dir = argv[2];
        }
            [[fallthrough]];
        case 2: {
            context.cmdType = argv[1];
        }
            [[fallthrough]];
        default:
            break;
    }

    FVADataProcessor cmdExecutor;
    int result = cmdExecutor.run(context, cfg);
    g_logfile.close();
    return result;
}
