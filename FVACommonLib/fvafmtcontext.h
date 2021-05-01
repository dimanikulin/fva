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

	public: //methods

		/*!
		* \brief it fills up the formatting context from configuration
		* \param cfg - Fva Configuration to fill up from
		* \returns it returns code of error if any or FVA_NO_ERROR if filling was successful
		*/
		FVA_EXIT_CODE fillFmtContextFromCfg(const FvaConfiguration& cfg);
};


#endif // FVAFMTCONTEXT_H
