#ifndef CLT_CONTEXT_H_
#define CLT_CONTEXT_H_

#include <QtCore/QString>

/*!
 * \brief to keep command line task context (+settings) that is known before task is started
 */
class CLTContext
{
	public:

		/*!
		 * \brief human - readable type of command
		 */
		QString cmdType;

		/*!
		 * \brief path to work folder of command to work in
		 */
		QString dir;

		/*!
		 * \brief does command work in read-only mode
		 */
		bool readOnly;

		/*!
		 * \brief a parameter to customize a command  - might be string or integer inside
		 */
		QString custom;

		/*!
		 * \brief is it recursive mode now
		 */
		bool recursive;

};
#endif // CLT_CONTEXT_H_

#ifdef _NOT_DEFINED_
		////////////////////////// COMMON CONFIGURATION  //////////////////////////////////////////

		/*!
		* \brief path to FVA Software root folder (where the SW is installed)
		*/
		QString	rootDir;

		/*!
		* \brief how many supported files should be in a folder
		*/
		unsigned int minCountFilesInADir;

		////////////////////////// RENAMING FILE OPTIONS   //////////////////////////////////////////
		/*!
		* \brief shall be renamed pictures files using the file modification time if they do not have exif taken time set
		*/
		bool renamePicsByModificationTimeIfEmptyExif;

		/*!
		* \brief shall be renamed video files using the file modification time if they do not have taken time set
		*/
		bool renameVideoByModificationTimeIfEmptyExif;

		////////////////////////// FORMATTING STRING OPTIONS/////////////////////////////////////////
		/*!
		* \brief format a dir name as a date
		*/
		QString dirNameFmt;

		/*!
		* \brief format a dir name as a year
		*/
		QString dirNameYearFmt;

		/*!
		* \brief format a FVA file name as a date + time
		*/
		QString fileNameFmt;

		/*!
		* \brief format an EXIF date + time
		*/
		QString ExidDateTimeFmt;
#endif // _NOT_DEFINED_