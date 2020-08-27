#ifndef FVAPEOPLE_H
#define FVAPEOPLE_H

#include <QtCore/QString>

/*!
 * \brief it keeps fva specific information about a file
 */
class fvaFile
{
	public:

		fvaFile():
			deviceId(0),
			scanerId(0),
			placeId(0)
		{
		}
		virtual ~fvaFile()
		{
		}

		/*!
		 * place id 
		 */
		unsigned int			placeId;

		/*!
		 * id of device 
		 */
		unsigned int			deviceId;

		/*!
		 * id of scaner
		 */
		unsigned int			scanerId;

		/*!
		 * people list ids 
		 */
		QVector<unsigned int>	peopleIds;

		/*!
		 * decsription
		 */
		QString					description;

		/*!
		 * file comment
		 */
		QString					comment;

		/*!
		 * name of file
		 */
		QString					name;

};

#endif // FVAPEOPLE_H