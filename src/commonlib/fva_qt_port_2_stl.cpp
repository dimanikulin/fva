/*!
 * \file fva_qt_port_2_stl.cpp
 * \copyright Copyright 2021 FVA Software. All rights reserved. This file is released under the XXX License.
 * \author Dima Nikulin.
 * \version 0.29
 * \date  2014-2021
 */
#include "fva_qt_port_2_stl.h"

#include <algorithm>
#include <cctype>

// TODO create UT for this function
std::string fvaStrToUpper(const std::string& str)  {
    std::string result = str;
    std::transform(result.begin(), result.end(), result.begin(), [](unsigned char c) { return std::toupper(c); });
    return result;
}
