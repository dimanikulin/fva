#ifndef _FVA_DESC_FILE_
#define _FVA_DESC_FILE_

#include "csvfile.h"
#include "fvacommonlib.h"

/*!
 * \brief it wraps functions to work with descption file (having format a csv file)
 */
class FVADescriptionFile : 	public CSVFile
{
	public:
	
		/*!
		 * \brief ctors-dtors section
		 */
		FVADescriptionFile( );
 		virtual ~FVADescriptionFile( );

		/*!
		 * \brief it loads data from file
		 * \param path - path to CSV file
		 * \param titles it is first row of a set, and it keeps names of columns
		 * \param decsItems it set of rows with structure according to titles
		 * \returns it returns code of error if any or FVA_NO_ERROR if loading was successful
		 */
		FVA_ERROR_CODE load( const QString& path, QStringList& titles, DESCRIPTIONS_MAP& decsItems );

		/*!
		 * \brief it saves data to file
		 * \param path - path to CSV file
		 * \param titles it is first row of a set, and it keeps names of columns
		 * \param decsItems it set of rows with structure according to titles
		 * \returns it returns code of error if any or FVA_NO_ERROR if saving was successful
		 */
		FVA_ERROR_CODE save( const QString& path, const QStringList& titles, const DESCRIPTIONS_MAP& decsItems );

		/*!
		 * \brief it returns id of colum by name 
		 */
		static int getColumnIdByName( const QStringList& titles, const QString& columnName );

};

#endif // _FVA_DESC_FILE_