#ifndef _RIFF_PARSER_H_
#define _RIFF_PARSER_H_

#include <Qstring> 
#include <QDateTime>
#include <QDataStream>
#include <QFile>


/*!
 * \brief contains functionality to worj with RIFF files
 * see https://en.wikipedia.org/wiki/Resource_Interchange_File_Format for the details
 */
class RiffParser
{
	public:
		/*!
		 * \brief ctors-dtors section
		 */
		RiffParser(  );
		virtual ~RiffParser( );

	public: // methods

		/*
		 * \brief it opens file and makes premialiry checks for consitensy
		 * \param error human-readable description of error if any
		 * \param path riff file absolute path
		 * \return it returns true if it succesfully opened the file, otherwize it returns false 
		 */
		bool open ( const QString& path, QString& error );

		/*!
		 * \brief it finds string type tag by its name
		 * \param tag - name of tag to be found
		 * \param value out param to be filled by value of tag 
		 * \return it returns true if it succesfully found the tag, otherwize it returns false 
		 */
		bool findTag( const QString& tag, QString& value );

		/*!
		 * \brief it converts string type to date-time type
		 * \param strDate - human-readable date string
		 * \param value - out param to be filled by date-time value 
		 * \return it returns true if it succesfully converted the value, otherwize it returns false 
		 */
		bool convertToDate ( const QString& strDate, QDateTime& value ) const;

	private: // methods

		/*!
		 * \brief it finds string type tag by its name inside of current node
		 * \param tag - name of tag to be found
		 * \param value out param to be filled by value of tag 
		 * \return it returns true if it succesfully found the tag, otherwize it returns false 
		 */
		bool processNode( const QString& tag, QString& value );

	private: // data

		/*!
		 * file system object
		 */
		std::auto_ptr< QFile >			m_file;

		/*!
		 * stream object
		 */
		std::auto_ptr< QDataStream >	m_stream;

};

#endif // _RIFF_PARSER_H_

