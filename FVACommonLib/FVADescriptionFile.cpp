#include "FVADescriptionFile.h"

FVADescriptionFile::FVADescriptionFile(  ) 
{
}

FVADescriptionFile::~FVADescriptionFile( )
{

}

FVA_ERROR_CODE FVADescriptionFile::load( const QString& path, QStringList& titles, DESCRIPTIONS_MAP& decsItems )
{
	if ( !openForRead( path ) )
		return FVA_ERROR_CANT_OPEN_FILE_DESC;
	
	int		indexOfFileNameColumn = -1;
	while ( !atEnd() )
	{
		QString	line = readLine();
		if ( line.isEmpty() && atEnd() )
			break;
		if ( line.trimmed().isEmpty() )
			continue;
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
FVA_ERROR_CODE FVADescriptionFile::save( const QString& path, const QStringList& titles, const DESCRIPTIONS_MAP& decsItems )
{
	if ( !openForWrite( path ) )
		return FVA_ERROR_CANT_OPEN_FILE_DESC;
	for ( auto i = decsItems.begin(); i !=decsItems.end(); ++i )
		if ( i.value().size() != titles.size() )
			return FVA_ERROR_INCORRECT_FORMAT;
	QString oneStr = titles.join(",");
	writeLine( oneStr );
	oneStr = "";
	for ( auto i = decsItems.begin(); i !=decsItems.end(); ++i )
	{
		for ( auto iv = i.value().begin(); iv != i.value().end(); ++iv )
		{
			if ( iv->contains(',') ) 
			{
				if ( oneStr.isEmpty() )
					oneStr += "\"" + iv->trimmed() + "\"";
					else
						oneStr += ",\"" + iv->trimmed() + "\"";	
			}
			else
			{
				if ( oneStr.isEmpty() )
					oneStr += iv->trimmed();
				else
					oneStr += "," + iv->trimmed();
			}
		}

		writeLine( oneStr.remove("\t") );
		oneStr = "";
	}
	return FVA_NO_ERROR;
}
