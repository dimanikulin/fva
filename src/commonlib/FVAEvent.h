/*!
 * \file FVAEvent.h
 * \copyright Copyright 2021 FVA Software. All rights reserved. This file is released under the XXX License.
 * \author Dima Nikulin.
 * \version 0.29
 * \date  2014-2022
 */
#ifndef FVAEVENT_H
#define FVAEVENT_H

#include <QtCore/QString>
#include <QtCore/QMap>

#include "fvaconstants.h"

/*!
 * \brief it describes whole information about event
 ID,Name,Type,Institution
 */
class fvaEvent
{
	public:
		/*
		 * event identifier from event dictionary - fvaEvents.csv
		 */
		int ID = 0;

		/*
		 * name 
		 */
		QString name;

		/*
		 * relation type from fvaRelationTypes.csv
		 */
		unsigned int type = 0;

		/*
		 * institution type from fvaInstitutions.csv
		 */
		unsigned int institution = 0;
};

typedef QMap<int, fvaEvent> FVA_EVENT_MAP;

#endif // FVAEVENT_H