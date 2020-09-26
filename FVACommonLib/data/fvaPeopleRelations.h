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