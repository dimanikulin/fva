#include "fvadescriptioneditor.h"
#include "fvadescriptionfile.h"
#include "fvacommonlib.h"

#include <QtGui/QApplication>
#include <QDir>
#include <QMessageBox>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	
	QString dictPath, imagePath; 
	QDir dir;
	if ( a.arguments().size() >= 3 )
	{
		dictPath	= a.arguments().at(1);
		imagePath	= a.arguments().at(2);
		dir			= QDir(imagePath);
		if ( !dir.exists( imagePath ) )
			return FVA_ERROR_CANT_OPEN_INPUT_FILE;
	}
	else if ( a.arguments().size() == 2 )
	{
		dictPath	= a.arguments().at(1);
		dir			= QDir ( a.applicationDirPath() + "/" + FVA_DICTIONARY_NAME );
	}
	else if ( a.arguments().size() == 1 )
	{
		dictPath	= a.applicationDirPath() + "/" + FVA_DICTIONARY_NAME;
		dir			= QDir ( a.applicationDirPath() + "/" + FVA_DICTIONARY_NAME );
	}

	QString		error;
	QVariantMap	dictionaries;
	FVA_ERROR_CODE res = fvaLoadDictionary( dictPath, dictionaries, error );
	if ( FVA_NO_ERROR != res )
		return res;

	dir.cdUp();

	std::auto_ptr<FVADescriptionFile> desc( new FVADescriptionFile );
	QStringList			titles; 
	DESCRIPTIONS_MAP	decsItems;

	res = desc->load( ( 
		(( a.arguments().size() == 1 ) 
		? a.applicationDirPath() 
		: dir.absolutePath() )
		+ "/" + FVA_DESCRIPTION_FILE_NAME ),
			titles, 
			decsItems );
	
	bool emptyDescFile = false;
	if ( FVA_ERROR_CANT_OPEN_FILE_DESC == res )
		emptyDescFile = true;
	else if ( FVA_NO_ERROR != res )
		return res;

	QStringList dirFiles;
	QStringList emptyDescription;
	int indexOfFile = -1;
	if (emptyDescFile)
	{
		titles.append("Name");			
		titles.append("Place");			
		titles.append("People");		
		titles.append("Device");		
		titles.append("WhoTook");		
		titles.append("Description");	
		titles.append("Scaner");		
		titles.append("Comment");		
		titles.append("oldName");		
	}

	for (auto iTitles = 0; iTitles != titles.size(); ++iTitles)
		emptyDescription.append("");

	Q_FOREACH(QFileInfo info, dir.entryInfoList(QDir::NoDotAndDotDot | QDir::System | QDir::Hidden  | QDir::Files, QDir::DirsFirst))
	{
		if ( info.isDir() )
			continue;

		// skip meta files
		if ( !fvaIsFVAFile(info.suffix().toUpper()) || fvaIsInternalFile( info.fileName() ) )
			continue;

		if ( imagePath == info.absoluteFilePath() )
			indexOfFile = dirFiles.size();
		
		dirFiles.append(info.absoluteFilePath());

		if (emptyDescFile)
		{
			emptyDescription[0]						= info.fileName().toUpper();
			decsItems[info.fileName().toUpper()]	= emptyDescription;	
		}
	}
	if ( -1 == indexOfFile && imagePath.isEmpty() )
		indexOfFile = 0;

	desc.reset( 0 );

	FVADescriptionEditor w( titles, decsItems, dictionaries, indexOfFile, dirFiles );
	w.show();
	return a.exec();
}
