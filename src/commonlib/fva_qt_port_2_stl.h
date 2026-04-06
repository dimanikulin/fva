#ifndef FVA_QT_PORT_2_STL_H
#define FVA_QT_PORT_2_STL_H

// TODO to create name space for this file and move it to commonlib/utils or something like that
/*!
 * \file fva_qt_port_2_stl.h
 * \copyright Copyright 2021 FVA Software. All rights reserved. This file is released under the XXX License.
 * \author Dima Nikulin.
 * \version 0.29
 * \date  2014-2021
 */

#include <string>

/*!
 * \brief Helper function to convert string to uppercase
 * \param str The input string
 * \return The uppercase version of the input string
 */
std::string fvaStrToUpper(const std::string& str);

/*!
 * \brief Helper function to quote a command-line argument
 * \param arg The input argument
 * \return The quoted version of the input argument
 */
std::string quoteArg(const std::string& arg);

/*!
 * \brief Helper function to check if a directory exists
 * \param dirPath The path to the directory
 * \return true if the directory exists, false otherwise
 */
bool isExistingDirectory(const std::string& dirPath);

#endif  // FVA_QT_PORT_2_STL_H
