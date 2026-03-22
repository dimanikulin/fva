/*!
 * \file FvaFmtContext.h
 * \copyright Copyright 2021 FVA Software. All rights reserved. This file is released under the XXX License.
 * \author Dima Nikulin.
 * \version 0.29
 * \date  2014-2021
 */
#ifndef FVAFMTCONTEXT_H
#define FVAFMTCONTEXT_H

#include <string>

#include "fvaconfiguration.h"

/*!
 * \brief to keep the set of setting for string formatting puspose
 */
class FvaFmtContext {
public:  // data
    /*!
     * \brief format a dir name as a date
     */
    std::string fvaDirName;

    /*!
     * \brief format a dir name as a year
     */
    std::string fvaDirNameYear;

    /*!
     * \brief format a FVA file name as a date + time
     */
    std::string fvaFileName;

    /*!
     * \brief format an EXIF date + time
     */
    std::string exifDateTime;

    /*!
     * \brief file name format 1 as a date + time
     */
    std::string fileName1;

    /*!
     * \brief file name format 2 as a date + time
     */
    std::string fileName2;

    /*!
     * \brief riff format 1 of date + time
     */
    std::string riffDateTime1;

    /*!
     * \brief riff format 2 of date + time
     */
    std::string riffDateTime2;

public:  // methods
    /*!
     * \brief it fills up the formatting context from configuration
     * \param cfg - Fva Configuration to fill up from
     * \returns it returns code of error if any or FVA_NO_ERROR if filling was successful
     */
    FVA_EXIT_CODE fillFmtContextFromCfg(const FvaConfiguration& cfg);
};

#endif  // FVAFMTCONTEXT_H
