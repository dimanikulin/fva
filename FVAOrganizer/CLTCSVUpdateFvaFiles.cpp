#include "CLTCSVUpdateFvaFiles.h"

#include "fvadefaultcfg.h"
#include "FVADescriptionFile.h"


CLTCSVUpdateFvaFiles::CLTCSVUpdateFvaFiles(const QString& dir_, bool readOnly_, const QString& custom_ )
:CmdLineBaseTask(dir_, readOnly_, custom_)
{
	LOG_QDEB << "cmd created,dir:" << dir_ << ",RO=" << (readOnly_ ? "yes" : "no") << ",SRO=" << (supportReadOnly() ? "yes" : "no");

	FVADescriptionFile fvaFileCsv;

	fvaFileCsv.load(FVA_DEFAULT_ROOT_DIR + "#data#/fvaFile.csv", m_fileTitles, m_fileDecsItems);

	int columnName = FVADescriptionFile::getColumnIdByName(m_fileTitles, "Name");

	for (auto it = m_fileDecsItems.begin(); it != m_fileDecsItems.end(); ++it)
	{
		QStringList list = it.value();
		QString fileName = list[columnName].toUpper();

		m_fileNameToID[fileName] = it.key();
	}

	FVADescriptionFile fvaDirCsv;

	fvaDirCsv.load(FVA_DEFAULT_ROOT_DIR + "#data#/fvaFolder.csv", m_dirTitles, m_dirDecsItems);

	for (auto it = m_dirDecsItems.begin(); it != m_dirDecsItems.end(); ++it)
	{
		QStringList list = it.value();
		QString dirName = list[columnName].toUpper();

		m_dirNameToID[dirName] = it.key();
	}
}
CLTCSVUpdateFvaFiles::~CLTCSVUpdateFvaFiles()
{ 
	FVADescriptionFile fvaFileCsv;

	fvaFileCsv.save(FVA_DEFAULT_ROOT_DIR + "#data#/fvaFileE.csv", m_fileTitles, m_fileDecsItems);

	LOG_QDEB << "cmd deleted, dir:" << m_folder; 
	if (m_dirNameToID.isEmpty())
		return;
	for (auto it = m_dirDecsItems.begin(); it != m_dirDecsItems.end(); ++it)
	{
		LOG_QDEB << "found not processed dir " << it.key();
	}
}
FVA_EXIT_CODE CLTCSVUpdateFvaFiles::execute()
{
	QString dirToMatch = m_dir.absolutePath();
	dirToMatch = dirToMatch.replace("\\", "/");  // replace slaches on backslashes
	dirToMatch = dirToMatch.remove(FVA_DEFAULT_ROOT_DIR); // remove a prefix as root dir
	dirToMatch = "/" + dirToMatch;

	auto itDirID = m_dirNameToID.find(dirToMatch);
	if (m_dirNameToID.end() == itDirID)
	{
		LOG_QDEB << "Skipped dir " << dirToMatch << " as not having fva info";
		return FVA_NO_ERROR;
	}
	int IDdir = itDirID.value();
	auto itDir = m_dirDecsItems.find(IDdir);
	if (m_dirDecsItems.end() == itDir)
		return FVA_ERROR_CANT_FIND_MANDATORY_FIELDS;

	QStringList dirList = itDir.value();

	// ID, Name, Reserved1, Tags, Reserved2, PlaceId, EventId, ReasonPeople, Reserved5, Reserved3, Reserved4
	int columnDirPlaceId = FVADescriptionFile::getColumnIdByName(m_dirTitles, "PlaceId");
	if (-1 == columnDirPlaceId)
		return FVA_ERROR_CANT_FIND_MANDATORY_FIELDS;

	int columnDirEventId = FVADescriptionFile::getColumnIdByName(m_dirTitles, "EventId");
	if (-1 == columnDirEventId)
		return FVA_ERROR_CANT_FIND_MANDATORY_FIELDS;

	int columnDirReasonPeople = FVADescriptionFile::getColumnIdByName(m_dirTitles, "ReasonPeople");
	if (-1 == columnDirReasonPeople)
		return FVA_ERROR_CANT_FIND_MANDATORY_FIELDS;

	int columnDirTags = FVADescriptionFile::getColumnIdByName(m_dirTitles, "Tags");
	if (-1 == columnDirTags)
		return FVA_ERROR_CANT_FIND_MANDATORY_FIELDS;

	// remove it from dictionary 
	itDirID = m_dirNameToID.erase(itDirID);

	Q_FOREACH(QFileInfo info, m_dir.entryInfoList(QDir::NoDotAndDotDot | QDir::System | QDir::Hidden | QDir::AllDirs | QDir::Files, QDir::DirsFirst))
	{
		if (info.isDir())
			continue;
		QString suffix = info.suffix().toUpper();
		if (!fvaIsFVAFile(info.suffix().toUpper()))
			continue;

		QString fileName = info.fileName();
		auto itFileID = m_fileNameToID.find(info.fileName().toUpper());
		if (m_fileNameToID.end() == itFileID)
		{
			LOG_QDEB << "found a file that was not in fva info" << fileName;
			return FVA_ERROR_CANT_FIND_MANDATORY_FIELDS;
		}
		int ID = itFileID.value();
		auto itFile = m_fileDecsItems.find(ID);
		if (m_fileDecsItems.end() == itFile)
			return FVA_ERROR_CANT_FIND_MANDATORY_FIELDS;
		QStringList fileList = itFile.value();

		// ID,Name,PlaceId,People,DevId,Description,ScanerId,Comment,EventId,ReasonPeople,Tags
		//////////////////////////////PLACEID///////////////////////////////////////////////
		int columnFilePlaceId = FVADescriptionFile::getColumnIdByName(m_fileTitles, "PlaceId");
		if (-1 == columnFilePlaceId)
			return FVA_ERROR_CANT_FIND_MANDATORY_FIELDS;

		if (fileList[columnFilePlaceId].trimmed().isEmpty())
		{
			if (!dirList[columnDirPlaceId].trimmed().isEmpty())
			{
				fileList[columnFilePlaceId] = dirList[columnDirPlaceId].trimmed();
				LOG_QDEB << "applied for a file " << fileName << ", PlaceID " << dirList[columnDirPlaceId].trimmed();
			}
		}
		else
			LOG_QDEB << "found a file that already has placeID " << fileName << ", " << fileList[columnFilePlaceId].trimmed();

		//////////////////////////////EVENTID///////////////////////////////////////////////
		int columnFileEventId = FVADescriptionFile::getColumnIdByName(m_fileTitles, "EventId");
		if (-1 == columnFileEventId)
			return FVA_ERROR_CANT_FIND_MANDATORY_FIELDS;
		if (fileList[columnFileEventId].trimmed().isEmpty())
		{
			if (!dirList[columnDirEventId].trimmed().isEmpty())
			{
				fileList[columnFileEventId] = dirList[columnDirEventId].trimmed();
				LOG_QDEB << "applied for a file " << fileName << ", EventId " << dirList[columnDirEventId].trimmed();
			}
		}
		else
			LOG_QDEB << "found a file that already has EVENTID" << fileName << ", " << fileList[columnFileEventId].trimmed();

		//////////////////////////////ReasonPeople///////////////////////////////////////////////
		int columnFileReasonPeople = FVADescriptionFile::getColumnIdByName(m_fileTitles, "ReasonPeople");
		if (-1 == columnFileReasonPeople)
			return FVA_ERROR_CANT_FIND_MANDATORY_FIELDS;
		if (fileList[columnFileReasonPeople].trimmed().isEmpty())
		{
			if (!dirList[columnDirReasonPeople].trimmed().isEmpty())
			{
				fileList[columnFileReasonPeople] = dirList[columnDirReasonPeople].trimmed();
				LOG_QDEB << "applied for a file " << fileName << ", ReasonPeople " << dirList[columnDirReasonPeople].trimmed();
			}
		}
		else
			LOG_QDEB << "found a file that already has ReasonPeople" << fileName << ", " << fileList[columnFileReasonPeople].trimmed();

		//////////////////////////////Tags///////////////////////////////////////////////
		int columnFileTags = FVADescriptionFile::getColumnIdByName(m_fileTitles, "Tags");
		if (-1 == columnFileTags)
			return FVA_ERROR_CANT_FIND_MANDATORY_FIELDS;
		if (fileList[columnFileTags].trimmed().isEmpty())
		{
			if (!dirList[columnDirTags].trimmed().isEmpty())
			{
				fileList[columnFileTags] = dirList[columnDirTags].trimmed();
				LOG_QDEB << "applied for a file " << fileName << ", tags " << dirList[columnDirTags].trimmed();
			}
		}
		else
			LOG_QDEB << "found a file that already has Tags" << fileName << ", " << fileList[columnFileTags].trimmed();

		m_fileDecsItems[ID] = fileList;
	}
	return FVA_NO_ERROR;
}