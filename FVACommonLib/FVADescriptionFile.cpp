#include "FVADescriptionFile.h"

FVADescriptionFile::FVADescriptionFile( const QString& path )
	: CSVFile ( path ) 
{
}

FVADescriptionFile::~FVADescriptionFile( )
{

}

FVA_ERROR_CODE FVADescriptionFile::load( QStringList& titles, DESCRIPTIONS_MAP& decsItems )
{
	if ( !open() )
		return FVA_ERROR_CANT_OPEN_FILE_DESC;
	
	int		indexOfFileNameColumn = -1;
	while ( !atEnd() )
	{
		QString	line = readLine();
		if ( line.isEmpty() && atEnd() )
			break;
		QStringList	values;
		if ( !parseLine( line, values ) )
			continue;
		else
		{
			if ( titles.isEmpty() )
			{
				indexOfFileNameColumn = getColumnIdByName( values, "name" );
				if ( -1 == indexOfFileNameColumn )
					return FVA_ERROR_CANT_FIND_MANDATORY_FIELDS;
				titles = values;
				continue;
			}
			else
			{
				if ( values.size() != titles.size() )
					return FVA_ERROR_INCORRECT_FORMAT;
				decsItems [ values[ indexOfFileNameColumn ].toUpper() ] = values;
			}
		}
	}
	return FVA_NO_ERROR;
}
int FVADescriptionFile::getColumnIdByName( const QStringList& titles, const QString& columnName )
{
	for ( int id = 0; id < titles.size(); ++id )
	{
		if ( columnName.toUpper() == titles[ id ].toUpper() )
			return id;
	}
	return -1;
}
