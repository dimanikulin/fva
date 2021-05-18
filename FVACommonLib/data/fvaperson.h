/*!
* \file fvaperson.h
* \copyright Copyright 2021 FVA Software. All rights reserved. This file is released under the XXX License.
* \author Dima Nikulin.
* \version 0.29
* \date  2014-2021
*/
#ifndef FVAPERSON_H
#define FVAPERSON_H

#include <QtCore/QString>

/*!
* \brief it describes whole information about a person
*/
class fvaPerson
{
public:

	/*!
	 * people identifier from a dictionary
	 */
	int Id;

	/*!
	 * name from dictionary - short form
	 */
	QString name;

	/*!
	 * name from dictionary - full form
	 */
	QString fullName;

	/*!
	 * relation people type identifier from a dictionary
	 */
	int relationId;

	/*!
	* person Id that is related to this one from a dictionary
	*/
	int relatedPersionId;
};

typedef QMap<int, fvaPerson> PEOPLE_MAP;

#endif // FVAPERSON_H