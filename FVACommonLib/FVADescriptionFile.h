#ifndef _FVA_DESC_FILE_
#define _FVA_DESC_FILE_

#include "csvfile.h"
#include "fvacommonlib.h"

typedef QMap< QString, QStringList > DESCRIPTIONS_MAP;

	/*!
 * \brief it wraps functions to work with descption file (having format a csv file)
 */
class FVADescriptionFile : 	public CSVFile
{
	public:
	
		/*!
		 * \brief ctors-dtors section
		 */
		FVADescriptionFile( const QString& path );
 		virtual ~FVADescriptionFile( );

		/*!
		 * \brief it loads data from file
		 * \param titles it is first row of a set, and it keeps names of columns
		 * \param decsItems it set of rows with structure according to titles
		 * \returns it returns code of error if any or FVA_NO_ERROR if loading was successful
		 */
		FVA_ERROR_CODE load( QStringList& titles, DESCRIPTIONS_MAP& decsItems );

		/*!
		 * \brief it return id of colum by name 
		 */
		static int getColumnIdByName( const QStringList& titles, const QString& columnName );

};

#endif // _FVA_DESC_FILE_