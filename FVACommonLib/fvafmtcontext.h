#ifndef FVAFMTCONTEXT_H
#define FVAFMTCONTEXT_H

#include <QtCore/QString>

#include "fvaconfiguration.h"

/*!
 * \brief to keep the set of setting for string formatting puspose
 */
class FvaFmtContext
{

	public: // data 
	
		/*!
		 * \brief format a dir name as a date
		 */
		QString		fvaDirName;

		/*!
		 * \brief format a dir name as a year
		 */
		QString		fvaDirNameYear;

		/*!
		 * \brief format a FVA file name as a date + time
		 */
		QString		fvaFileName;

		/*!
		 * \brief format an EXIF date + time
		 */
		QString		exifDateTime;

		/*!
		 * \brief file name format 1 as a date + time
		 */
		QString		fileName1;

		/*!
		 * \brief file name format 2 as a date + time
		 */
		QString		fileName2;

		/*!
		 * \brief riff format 1 of date + time
		 */
		QString		riffDateTime1;

		/*!
		 * \brief riff format 2 of date + time
		 */
		QString		riffDateTime2;

	public: //methods

		/*!
		* \brief it fills up the formatting context from configuration
		* \param cfg - Fva Configuration to fill up from
		* \returns it returns code of error if any or FVA_NO_ERROR if filling was successful
		*/
		FVA_EXIT_CODE fillFmtContextFromCfg(const FvaConfiguration& cfg);
};


#endif // FVAFMTCONTEXT_H
