#ifndef FVAPEOPLE_H
#define FVAPEOPLE_H

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

#endif // FVAPEOPLE_H