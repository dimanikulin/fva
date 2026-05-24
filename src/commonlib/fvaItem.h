/*!
 * \file fvaitem.h
 * \copyright Copyright 2021 FVA Software. All rights reserved. This file is released under the XXX License.
 * \author Dima Nikulin.
 * \version 0.29
 * \date  2014-2021
 */
#ifndef FVAITEM_H
#define FVAITEM_H

#include <chrono>
#include <list>
#include <map>
#include <string>

#include "fvaconstants.h"

class fvaFolder;
class fvaFile;

/*!
 * \brief it keeps whole information about a foto-video-audio element
 */
class fvaItem {
public:
    fvaItem();
    virtual ~fvaItem();

    /*!
     * \brief it returns human-readable string as item name
     * \param dictionaries - global dictionary set
     */
    std::string getGuiName(const std::map<std::string, std::any>& dictionaries);

    /*!
     * \brief it returns human-readable string to show full name
     * \param dictionaries - global dictionary set
     */
    std::string getGuiFullName(const std::map<std::string, std::any>& dictionaries);

public:  // data
    /*!
     * fs type
     */
    FVA_FS_TYPE type;

    /*!
     * for folder it is event date period, for file it is date taken(dateFrom only)
     * both are filesystem names
     */
    std::chrono::system_clock::time_point dateFrom;
    std::chrono::system_clock::time_point dateTo;

    /*!
     * children
     */
    std::list<fvaItem*> children;

    /*!
     * absolute path to item
     */
    std::string fsFullPath;

    /*!
     * does this item fit filtration conditions
     */
    bool isFiltered;

    /*!
     * folder fva specific info
     */
    fvaFolder* pFvaFolder;

    /*!
     * file fva specific info
     */
    fvaFile* pFvaFile;
};

#endif  // FVAITEM_H
