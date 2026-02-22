/*!
* \file FvaCSVFile.h
* \copyright Copyright 2021 FVA Software. All rights reserved. This file is released under the XXX License.
* \author Dima Nikulin.
* \version 0.29
* \date  2014-2021
*/

#ifndef _CSV_FILE_HEADER_
#define _CSV_FILE_HEADER_

#include <memory>
#include <QtCore/QString>
#include <QtCore/QFile>
#include <QtCore/QTextStream>
#include <QtCore/QStringList>

/*!
 * \brief it keeps functionality to work with a csv file
 */
class CSVFile
{
	protected:
		/*!
		 * \brief ctors-dtors section
		 */
		CSVFile( );
		virtual ~CSVFile( );
	
	protected : // methods
		/*!
		 * \brief it open csv file for readnig 
		 * \return it returns true if opening was successfull, otherwize it returns false 
		 */
		bool openForRead ( const QString& path );

		/*!
		 * \brief it open csv file for writing 
		 * \return it returns true if opening was successfull, otherwize it returns false 
		 */
		bool openForWrite ( const QString& path );

		/*!
		 * \brief it reads one string terminated OA or OAOD from file
		 * \return it returns qstring if reading was successfull, otherwize it returns empty string 
		 */
		QString readLine( );

		/*!
		 * \brief it answers if file is at the end 
		 */
		bool atEnd( ); 

		/*!
		 * \brief it tries to parse line got
		 * \param line - string to be parsed
		 * \param values - output array of string parsed
		 * \param delimiter - delimiter of the fields to tokenize
		 * \return it returns true if it parsed line succesfully, otherwise it returns false
		 */
		bool parseLine( const QString& line, QStringList& values, char delimiter = ',' );

		/*!
		 * \brief it writes line to a file 
		 * \param line - string to be written
		 * \return it returns true if it has been written succesfully, otherwise it returns false
		 */
		bool writeLine ( const QString& line );

	private: // data
		/*!
		 * file system object
		 */
		std::unique_ptr< QFile >	m_file;

		/*!
		 * to work with file as with text one
		 */
		std::unique_ptr< QTextStream >	m_stream;
};

#endif // _CSV_FILE_HEADER_

