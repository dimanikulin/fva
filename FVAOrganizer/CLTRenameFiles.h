#ifndef _CLT_RENAME_FILES_H_
#define _CLT_RENAME_FILES_H_

#include "CmdLineBaseTask.h"

// in read only mode this cmd just checks if renaming is even possible
class CLTRenameFiles : public CmdLineBaseTask
{
public:
	/// CmdLineBaseTask interface
	CLTRenameFiles(const QString& dir_, bool readOnly_ = false, const QString& custom_ = "")
	:CmdLineBaseTask(dir_, readOnly_, custom_)
									{	LOG_QDEB << "cmd created, dir:" << dir_;		}
	virtual ~CLTRenameFiles()		{	LOG_QDEB << "cmd deleted, dir:" << m_folder;	}
	virtual FVA_EXIT_CODE execute();
	static QString Name()			{	return "CLTRenameFiles";						}
	virtual bool supportReadOnly()	{	return true;									}

protected:

	bool checkIfParentFileExist(const QFileInfo& fileToCheck, QDateTime& renameDateTime, const QDateTime& prevRenameDateTime);

	void fillRenameDateTimeFromLastModifiedIfValid(const QDir& dir, const QFileInfo& info, QDateTime& renameDateTime);
}; 

#endif // _CLT_RENAME_FILES_H_