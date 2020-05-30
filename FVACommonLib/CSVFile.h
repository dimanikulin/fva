#ifndef _CSV_FILE_HEADER_
#define _CSV_FILE_HEADER_

#include <QString>
#include <QFile>
#include <QTextStream>
#include <QStringList>

/*!
 * \brief it keeps functionality to work with a csv file
 */
class CSVFile
{
	protected:

		/*!
		 * \brief ctors-dtors section
		 */
		CSVFile( const QString& path );
		virtual ~CSVFile( );
	
	protected : // methods

		/*!
		 * \brief it open csv file 
		 * \return it returns true if opening was successfull, otherwize it returns false 
		 */
		bool open ( );

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

	private: // data

		/*!
		 * path to csv file
		 */
		QString							m_path; 

		/*!
		 * file system object
		 */
		std::auto_ptr< QFile >			m_file;

		/*!
		 * to work with file as with text one
		 */
		std::auto_ptr< QTextStream >	m_stream;
};

#endif _CSV_FILE_HEADER_

