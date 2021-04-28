#include "CLTDBFvaFileToDB.h"

#include "FVADescriptionFile.h"

QString FVget(FVADescriptionFile* desc, const QString& fieldName, const QStringList& titles, DESCRIPTIONS_MAP::Iterator it, QStringList& titlesToCheck)
{
	desc;
	int indexColumn = desc->getColumnIdByName(titles, fieldName);
	if (-1 == indexColumn)
	{
		// qCritical() << "could not find " << fieldName << " column in description file";
		return "";
	}
	titlesToCheck[indexColumn] = "";
	return it.value()[indexColumn];
}

CLTDBFvaFileToDB::~CLTDBFvaFileToDB()
{
	/*SaveSQL("13.fvaFile.sql");

	for (auto it = m_movedFiles.begin(); it != m_movedFiles.end(); ++it)
	{
		QFileInfo info(*it);
		if (!m_dir.rename(*it + "/" + FVA_DESCRIPTION_FILE_NAME,
			m_folder + "/" + info.fileName() + ".csv"))
		{
			LOG_QCRIT << "can not move file fva desc for " << *it;
			continue;
		}
		LOG_QWARN << "moved from - " << *it + "/" + FVA_DESCRIPTION_FILE_NAME
			<< " to - " << m_folder + "/" + info.fileName() + ".csv";
	}

	LOG_QWARN << "totally inserted - " << m_SQLs.size() / 2 << ", totally moved - " << m_movedFiles.size();*/
}

FVA_EXIT_CODE CLTDBFvaFileToDB::execute()
{
	/*std::auto_ptr<FVADescriptionFile> desc(new FVADescriptionFile);
	QStringList			titles;
	DESCRIPTIONS_MAP	decsItems;
	FVA_EXIT_CODE res = desc->load(m_folder + "/" + FVA_DESCRIPTION_FILE_NAME, titles, decsItems);
	RET_RES_IF_RES_IS_ERROR // yes, it is NOT an error for this CLT

		QStringList			t2Check = titles; // titlesToCheck 
	for (DESCRIPTIONS_MAP::Iterator it = decsItems.begin(); it != decsItems.end(); ++it)
	{
		// ID,Name,PlaceId,People,DevId,Description,ScanerId,Comment,OldName,WhoTook,OldName1
		QString insert = "insert into fvaFile values ((select max(ID)+1 from fvaFile),\""
			+ FVget(desc.get(), "Name", titles, it, t2Check).toUpper() + "\",\""
			+ FVget(desc.get(), "Place", titles, it, t2Check) + "\",\""
			+ FVget(desc.get(), "People", titles, it, t2Check) + "\",\""
			+ FVget(desc.get(), "Device", titles, it, t2Check) + "\",\""
			+ FVget(desc.get(), "Description", titles, it, t2Check) + "\",\""
			+ FVget(desc.get(), "Scaner", titles, it, t2Check) + "\",\""
			+ FVget(desc.get(), "Comment", titles, it, t2Check) + "\",\""
			+ FVget(desc.get(), "oldName", titles, it, t2Check).toUpper() + "\",\""
			+ FVget(desc.get(), "WhoTook", titles, it, t2Check) + "\",\""
			+ FVget(desc.get(), "oldName1", titles, it, t2Check).toUpper() + "\");";

		m_SQLs.push_back(insert);

		QString update;
		QFileInfo info(m_folder);
		update = "update fva set FvaFileId = (select max(ID) from fvaFile) where Path || \"/\" ||  Name = \""
			+ info.absoluteFilePath().toUpper()
			+ "/"
			+ FVget(desc.get(), "Name", titles, it, t2Check).toUpper()
			+ "\";";
		m_SQLs.push_back(update);
	}

	for (int id = 0; id < t2Check.size(); ++id)
	{
		if (t2Check[id].length() > 0)
			return FVA_ERROR_INCORRECT_FORMAT;
	}
	// move dir description file for future backup purpose
	m_movedFiles.push_back(m_folder);

	LOG_QWARN << "converted file description to SQL:" << m_dir.dirName();*/
	return FVA_NO_ERROR;
}
