#include "CLTDBCreateFVA.h"

FVA_EXIT_CODE CLTDBCreateFVA::execute()
{
	/*Q_FOREACH(QFileInfo info, m_dir.entryInfoList(QDir::NoDotAndDotDot | QDir::System | QDir::Hidden | QDir::AllDirs | QDir::Files, QDir::DirsFirst))
	{
		// just skip internal folder
		if ((info.isDir() && info.fileName()[0] == '#' && info.fileName()[info.fileName().size() - 1] == '#')
			||
			info.isFile() && !fvaIsFVAFile(info.suffix().toUpper()))
		{
			qDebug() << "skipped internal fs object - " << info.absoluteFilePath();
			continue;
		}
		// ID,Name,PlaceId,People,DevId,Description,ScanerId,Comment,OldName,WhoTook,OldName1
		QString insert = "insert into fvaFile values ((select max(ID)+1 from fvaFile),\"\",\"\",\"\",\""
			+ m_custom + "\",\"\",\"\",\"\",\"\",\"\",\"\");"; // m_custom here is device id

		m_SQLs.push_back(insert);

		QString update;
		update = "update fva set FvaFileId = (select max(ID) from fvaFile) where Path || \"/\" ||  Name = \""
			+ FVA_TARGET_FOLDER_NAME
			+ "/"
			+ info.fileName().toUpper()
			+ "\";";
		m_SQLs.push_back(update);
	}
	*/
	return FVA_NO_ERROR;
}
CLTDBCreateFVA::~CLTDBCreateFVA()
{
	SaveSQL("13.fvaFile.sql");

	LOG_QWARN << "totally inserted - " << m_SQLs.size() / 2;
}