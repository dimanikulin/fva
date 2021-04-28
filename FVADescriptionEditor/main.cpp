#include "fvadescriptioneditor.h"
#include "fvadescriptionfile.h"
#include "fvacommonlib.h"
#include "fvadefaultcfg.h"

#include <QtWidgets/QApplication>
#include <QtCore/QDir>
#include <QtWidgets/QMessageBox>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	
	bool folderMode = true;

	FVADescriptionEditor w( folderMode, nullptr);
	w.show();
	return a.exec();
}
/*
FVADescriptionEditor w( folderMode, titles, decsItems, dictionaries, 0, dirFiles, dictPath, fvaPath nullptr);

QString dictPath, fvaPath;
QDir dir;
if ( a.arguments().size() >= 3 )
{
dictPath	= a.arguments().at(1);
fvaPath		= a.arguments().at(2);
dir			= QDir(fvaPath);
if ( !dir.exists( fvaPath ) )
return FVA_ERROR_CANT_OPEN_INPUT_FILE;
QFileInfo info(fvaPath);
folderMode = info.isDir();
}
else if ( a.arguments().size() == 2 )
{
dictPath	= a.arguments().at(1);
dir			= QDir ( a.applicationDirPath() + "/" + FVA_DB_NAME );
folderMode	= true;
}
else if ( a.arguments().size() == 1 )
{
dictPath	= a.applicationDirPath() + "/" + FVA_DB_NAME;
dir			= QDir ( a.applicationDirPath() + "/" + FVA_DB_NAME );
folderMode	= true;
}

QString		error;
QVariantMap	dictionaries;
FVA_EXIT_CODE res = fvaLoadDictionary( dictPath, dictionaries, error );
RET_RES_IF_RES_IS_ERROR
QStringList			titles;
DESCRIPTIONS_MAP	decsItems;
bool emptyDescFile = false;

if (!folderMode)
{
dir.cdUp();

std::auto_ptr<FVADescriptionFile> desc( new FVADescriptionFile );

res = desc->load( (
(( a.arguments().size() == 1 )
? a.applicationDirPath()
: dir.absolutePath() )
+ "/" + FVA_DESCRIPTION_FILE_NAME ),
titles,
decsItems );

if ( FVA_ERROR_CANT_OPEN_FILE_DESC == res )
emptyDescFile = true;
else RET_RES_IF_RES_IS_ERROR

desc.reset( 0 );
}

QStringList dirFiles;
QStringList emptyDescription;
int indexOfFile = -1;
if (emptyDescFile || folderMode)
{
titles.append("Name");
titles.append("Place");
titles.append("People");
titles.append("Device");
titles.append("Description");
titles.append("Scaner");
titles.append("Comment");
titles.append("oldName");
}

if (!folderMode)
{
for (auto iTitles = 0; iTitles != titles.size(); ++iTitles)
emptyDescription.append("");

Q_FOREACH(QFileInfo info, dir.entryInfoList(QDir::NoDotAndDotDot | QDir::System | QDir::Hidden  | QDir::Files, QDir::DirsFirst))
{
if ( info.isDir() )
continue;

// skip meta files
if ( !fvaIsFVAFile(info.suffix().toUpper()) || fvaIsInternalFile( info.fileName() ) )
continue;

if ( fvaPath == info.absoluteFilePath() )
indexOfFile = dirFiles.size();

dirFiles.append(info.absoluteFilePath());

if (emptyDescFile)
{
emptyDescription[0]						= info.fileName().toUpper();
// decsItems[info.fileName().toUpper()]	= emptyDescription;
}
}
if ( -1 == indexOfFile && fvaPath.isEmpty())
indexOfFile = 0;
// FVADescriptionEditor w( folderMode, titles, decsItems, dictionaries, indexOfFile, dirFiles, dictPath, "" );
// w.show();
return a.exec();
}
else
{
QVariantMap		result;
QString			error;
//FVA_EXIT_CODE	code = fvaGetFolderDescription( fvaPath, result, error );
// RET_IF_RES_NO_ERROR
// "Name","Place","People","Device","Description","Scaner","Comment","oldName"
QFileInfo info(fvaPath);
emptyDescription.append(info.fileName().toUpper());
emptyDescription.append(result["place"].toString());
emptyDescription.append(result["people"].toString());
emptyDescription.append(result["deviceId"].toString());
emptyDescription.append(result["event"].toString());
emptyDescription.append(result["reasonPeople"].toString());
emptyDescription.append(result["tags"].toString());
emptyDescription.append(result["linkedFolder"].toString());

//decsItems[info.fileName().toUpper()]	= emptyDescription;

//FVADescriptionEditor w( folderMode, titles, decsItems, dictionaries, 0, dirFiles, dictPath, fvaPath );
//w.show();
return a.exec();
}*/

