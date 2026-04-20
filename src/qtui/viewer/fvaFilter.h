/*!
 * \file fvaFilter.h
 * \copyright Copyright 2021 FVA Software. All rights reserved. This file is released under the XXX License.
 * \author Dima Nikulin.
 * \version 0.29
 * \date  2014-2021
 */
#ifndef FVAFILTER_H
#define FVAFILTER_H

#include <QtCore/QDateTime>
#include <QtCore/QString>
#include <vector>

/*!
 * it keeps data to perform filtration
 */
class fvaFilter {
public:
    /*!
     * date to filter from
     */
    QDateTime dateFrom;

    /*!
     * date to filter to
     */
    QDateTime dateTo;

    /*!
     * places to be filtered
     */
    std::vector<unsigned int> placeIds;

    /*!
     * people to be entered
     */
    std::vector<unsigned int> peopleIds;

    /*!
     * list of id of foto devices
     */
    std::vector<unsigned int> deviceIds;

    /*!
     * folder events
     */
    std::vector<unsigned int> eventIds;

    /*!
     * people that are reason of event
     */
    std::vector<unsigned int> eventReasonPeopleIds;

    /*!
     * folder tags or file desciption/comment
     */
    QString text;

    /*
     * does ID match to filtration
     */
    bool doesIDMatchToFilter(unsigned int ID, const std::vector<unsigned int>& Ids) const {
        for (auto it = Ids.begin(); it != Ids.end(); ++it) {
            if (ID == *it) return true;
        }
        return false;
    }

    /*
     * does list of ID matche to filtration
     */
    bool doIDsMatchToFilter(const std::vector<unsigned int>& IDs, const std::vector<unsigned int>& filterIds) const {
        for (auto it = IDs.begin(); it != IDs.end(); ++it) {
            if (doesIDMatchToFilter(*it, filterIds)) return true;
        }
        return false;
    }
};

#endif  // FVAFILTER_H
