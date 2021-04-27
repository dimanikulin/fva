#include "CLTDBFvaFolderToDB.h"

CLTDBFvaFolderToDB::~CLTDBFvaFolderToDB()
{
	/*SaveSQL("12.fvaFolder.sql");

	for (auto it = m_movedFolders.begin(); it != m_movedFolders.end(); ++it)
	{
		QFileInfo info(*it);
		if (!m_dir.rename(*it + "/" + FVA_DIR_DESCRIPTION_FILE_NAME,
			m_folder + "/" + info.fileName() + ".json"))
		{
			LOG_QCRIT << "can not move fva desc for " << *it;
			continue;
		}
		LOG_QWARN << "moved from - " << *it + "/" + FVA_DIR_DESCRIPTION_FILE_NAME
			<< " to - " << m_folder + "/" + info.fileName() + ".json";
	}

	LOG_QWARN << "totally inserted - " << m_SQLs.size() / 2 << ", totally moved - " << m_movedFolders.size();
	*/
}

FVA_EXIT_CODE CLTDBFvaFolderToDB::execute()
{
	QVariantMap		dirDesc;
	QString			error;
	/*FVA_EXIT_CODE	code = fvaGetFolderDescription( m_folder, dirDesc, error );
	RET_IF_RES_NO_ERROR // yes, it is NOT an error for this CLT
	*/
	//ID,Name,DevId,Tags,People,PlaceId,EventId,ReasonPeople,LinkedFolder
	QString insert = "insert into fvaFolder values ((select max(ID)+1 from fvaFolder),\""
		+ m_dir.dirName() + "\","
		+ fvaDVget("deviceId", dirDesc) + ",\""
		+ fvaDVget("tags", dirDesc) + "\",\""
		+ fvaDVget("people", dirDesc) + "\",\""
		+ fvaDVget("place", dirDesc) + "\",\""
		+ fvaDVget("event", dirDesc) + "\",\""
		+ fvaDVget("reasonPeople", dirDesc) + "\",\""
		+ fvaDVget("linkedFolder", dirDesc) + "\",\""
		+ fvaDVget("whoTookFotoId", dirDesc) + "\",\""
		+ fvaDVget("scaner", dirDesc) + "\");";
	m_SQLs.push_back(insert);
	QString update;
	QFileInfo info(m_folder);
	update = "update fva set FvaFolderId = (select max(ID) from fvaFolder) where Path || \"/\" ||  Name = \"" + info.absoluteFilePath() + "\";";
	m_SQLs.push_back(update);

	if (dirDesc.size())
	{
		return FVA_ERROR_INCORRECT_FORMAT;
	}

	// move dir description file for future backup purpose
	m_movedFolders.push_back(m_folder);
	LOG_QWARN << "converted folder description to SQL:" << m_folder;
	return FVA_NO_ERROR;
}