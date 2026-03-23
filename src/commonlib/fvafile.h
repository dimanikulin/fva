/*!
 * \file fvafile.h
 * \copyright Copyright 2021 FVA Software. All rights reserved. This file is released under the XXX License.
 * \author Dima Nikulin.
 * \version 0.29
 * \date  2014-2021
 */

#ifndef FVAFILE_H
#define FVAFILE_H

#include <QtCore/QVector>

#include <map>
#include <string>

#include "fvaconstants.h"

/*!
 * \brief it keeps fva specific information about a file
 */
class fvaFile {
public:
    fvaFile()
        : deviceId(FVA_UNDEFINED_ID),
          scanerId(FVA_UNDEFINED_ID),
          eventId(FVA_UNDEFINED_ID),
          placeId(FVA_UNDEFINED_ID) {}

    virtual ~fvaFile() {}

    /*!
     * event id
     */
    unsigned int eventId;

    /*!
     * place id
     */
    unsigned int placeId;

    /*!
     * id of device
     */
    unsigned int deviceId;

    /*!
     * id of scaner
     */
    unsigned int scanerId;

    /*!
     * people list ids
     */
    QVector<unsigned int> peopleIds;

    /*!
     * decsription
     */
    std::string description;

    /*!
     * file comment
     */
    std::string comment;

    /*!
     * name of file
     */
    std::string name;

    /*!
     * list of people ids that caused this event
     */
    QVector<unsigned int> eventPeopleIds;
};
typedef std::map<std::string, fvaFile> FVA_FILE_INFO_MAP;
typedef std::map<int, fvaFile*> FVA_FILE_MAP;

#endif  // FVAFILE_H