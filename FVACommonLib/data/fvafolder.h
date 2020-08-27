#ifndef FVAFOLDER_H
#define FVAFOLDER_H

#include <QtCore/QString>
#include <QtCore/QVector> 

/*!
 * \brief it keeps fva specific information about a folder
 */
class fvaFolder
{
	public:

		fvaFolder() { eventId = 0;};
		virtual ~fvaFolder() {};

		/*!
		 * event id 
		 */
		unsigned int			eventId;

		/*!
		 * people that are reason of event
		 */
		QVector<unsigned int>	eventReasonPeopleIds;

		/*!
		 * folder tags
		 */
		QString					tags;

		/*!
		 * folder to show together with this folder
		 */
		QString					linkedFolder;
};

typedef QMap<int, fvaFolder*> FVA_FOLDER_MAP;

#endif // FVAFOLDER_H