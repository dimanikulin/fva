/*!
* \file fvafile.h
* \copyright Copyright 2021 FVA Software. All rights reserved. This file is released under the XXX License.
* \author Dima Nikulin.
* \version 0.29
* \date  2014-2021
*/

#ifndef FVAFILE_H
#define FVAFILE_H

#include <QtCore/QString>
#include <QtCore/QVector>

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
typedef QMap<QString, fvaFile> FVA_FILE_INFO_MAP;
typedef QMap<int, fvaFile*> FVA_FILE_MAP;

#endif // FVAFILE_H