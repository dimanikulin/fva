#ifndef FVAPEOPLE_H
#define FVAPEOPLE_H

#include <QtCore/QString>

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

#endif // FVAPEOPLE_H