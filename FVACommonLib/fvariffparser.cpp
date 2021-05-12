#include "fvariffparser.h"

bool RiffParser::open(const QString& path, QString& error)
{
	m_file.reset(new QFile(path));
	if (!m_file->open(QIODevice::ReadOnly))
	{
		error = "Can not open file";
		return false;
	}

	m_stream.reset(new QDataStream(m_file.get()));

	char buffer[12];

	// check format
	m_stream->readRawData(buffer, sizeof(buffer));
	if (QString::fromLatin1(buffer, 4) != "RIFF"
		&& QString::fromLatin1(buffer, 4) != "RIFX")
	{
		error = "Incorrect FourCC";
		return false;
	}

	qint64 datasize = *((int*)(&buffer[4])) + sizeof (qint64);
	// check size
	if (m_file->size() < datasize)
	{
		error = "Incorrect size";
		return false;
	}
	return true;
}
bool RiffParser::processNode( const QString& tag, QString& value )
{
	while ( !m_stream->atEnd() )
	{	
		// Examine the element, is it a list or a chunk
		char buffer [8];		
		m_stream->readRawData( buffer, sizeof( buffer ) );
		int datasize	= * ( (int*) ( &buffer[ 4 ]) );
		if ( QString::fromLatin1( buffer, 4 ) == "LIST" )
		{
			char buffer_ [4];
			m_stream->readRawData( buffer_, sizeof( buffer_ ) );	
			if ( processNode( tag, value ) )
				return true; // found wanted element
		}
		else
		{
			int paddedSize = datasize;
			if (0 != (datasize & 1)) 
				++paddedSize;
			if ( tag == QString::fromLatin1( buffer, 4 ) )
			{
				char buffer_ [1024];
				m_stream->readRawData( buffer_, sizeof( buffer_ ) );	
				value = QString::fromLatin1( buffer_, datasize );
				return true;
			}
			m_stream->skipRawData( paddedSize );	
		}
	}
	return false;
}
bool RiffParser::findTag( const QString& tag, QString& value )
{
	return processNode( tag, value );
}
bool RiffParser::convertToDate(const QString& strDate, QDateTime& value, const FvaFmtContext& ctx) const
{
	// remove terminate char and last symbol
	value = QDateTime::fromString( strDate.mid( 4, strDate.size() - 5 ).remove("\n"), ctx.riffDateTime1 );
	if ( !value.isValid() )
	{
		// try to use other format
		QString newStr = strDate.mid(0,strDate.size() -1 ).remove( "\n" );
		value = QDateTime::fromString(newStr, ctx.exifDateTime);
	}

	if ( !value.isValid() )
	{
		// try to use other format
		QString newStr = strDate.mid( QString ( "UTC " ).size() ).remove( "\n" );
		value = QDateTime::fromString(newStr, ctx.riffDateTime2);
	}

	return value.isValid();
}
