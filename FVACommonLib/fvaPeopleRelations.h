/*!
* \file fvapeoplerelations.h
* \copyright Copyright 2021 FVA Software. All rights reserved. This file is released under the XXX License.
* \author Dima Nikulin.
* \version 0.29
* \date  2014-2021
*/
#ifndef FVA_PEOPLE_RELATIONS_H
#define FVA_PEOPLE_RELATIONS_H

#include <QtCore/QString>

class fvaPeopleRelation
{
public:
	/*!
	 * identifier from a dictionary
	 */
	int Id;

	/*!
	* name from dictionary - short form
	*/
	QString name;

	/*!
	 * relation Type from dictionary (fvaRelationTypes.csv) - full form
	 */
	int relationType;
};

typedef QMap<int, fvaPeopleRelation> FVA_PEOPLE_RELATION_MAP;

#endif // FVA_PEOPLE_RELATIONS_H