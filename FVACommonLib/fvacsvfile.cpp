/*!
* \file fvacsvfile.cpp
* \copyright Copyright 2021 FVA Software. All rights reserved. This file is released under the XXX License.
* \author Dima Nikulin.
* \version 0.29
* \date  2014-2021
*/

#include "fvacsvfile.h"

#include <QtCore/QTextStream>
#include <QtCore/QTextCodec>

CSVFile::CSVFile(  )
{
}

CSVFile::~CSVFile(  )
{
	if ( m_stream.get() )
	{
		m_stream->flush();
		m_stream.release();
	}

	if ( m_file.get() )
	{
		m_file->close();
		m_file.release();
	}
}

bool CSVFile::openForRead( const QString& path )
{
	m_file.reset ( new QFile ( path ) );
	if ( !m_file->open( QIODevice::ReadOnly ) )
		return false;

	m_stream.reset ( new QTextStream( m_file.get() ) );
	m_stream->setCodec( QTextCodec::codecForName("UTF-8") );
	return true;
}

bool CSVFile::openForWrite( const QString& path )
{
	m_file.reset ( new QFile ( path ) );
	if ( !m_file->open( QIODevice::WriteOnly ) )
		return false;

	m_stream.reset ( new QTextStream( m_file.get() ) );
	m_stream->setCodec(QTextCodec::codecForName("UTF-8"));
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
	int			pos		= 0;
	bool		quotes	= false;
	QString		field	= "";
 
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

bool CSVFile::writeLine ( const QString& line )
{
	*m_stream << line << "\n";
	return true;
}
