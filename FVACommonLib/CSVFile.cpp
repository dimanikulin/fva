#include "CSVFile.h"


CSVFile::CSVFile( const QString& path )
	: m_path ( path )
{

}


CSVFile::~CSVFile(  )
{

}

bool CSVFile::open()
{
	m_file.reset ( new QFile ( m_path ) );
	if ( !m_file->open( QIODevice::ReadOnly ) )
		return false;

	m_stream.reset ( new QTextStream( m_file.get() ) );

	return true;
}

QString CSVFile::readLine( )
{
	return  m_stream->readLine( );
}

bool CSVFile::atEnd( )
{
	return m_stream->atEnd();
}

bool CSVFile::parseLine( const QString& line, QStringList&	values, char delimiter )
{
	unsigned int	pos		= 0;
	bool			quotes	= false;
	QString			field	= "";
 
	while( pos < line.length() && line[ pos ] != 0x00 )
	{
		QChar c = line[ pos ];
		if ( !quotes && c == '"' )
			quotes = true;
		else if ( quotes && c== '"' )
		{
			if ( pos + 1 < line.length() && line[ pos+1 ]== '"' )
			{
				field.push_back( c );
				pos++;
			}
			else
				quotes = false;        
		} 
		else if ( !quotes && c == delimiter ) 
		{
			values.push_back( field );
			field.clear();
		} 
		else if ( !quotes && ( c == 0x0A || c == 0x0D ) )
		{
			values.push_back( field );
			field.clear();
		}
		else 
			field.push_back( c ); 
		pos++;
		if ( pos >= line.size() ) // it was last symbol
			values.push_back( field );
	}  
 	
	return true;
}
int CSVFile::getColumnIdByName( const QStringList& titles, const QString& columnName )
{
	for ( int id = 0; id < titles.size(); ++id )
	{
		if ( columnName.toUpper() == titles[ id ].toUpper() )
			return id;
	}
	return -1;
}

