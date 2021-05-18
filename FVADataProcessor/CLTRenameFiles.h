/*!
* \file CLTRenameFiles.h
* \copyright Copyright 2021 FVA Software. All rights reserved. This file is released under the XXX License.
* \author Dima Nikulin.
* \version 0.29
* \date  2014-2021
*/
#ifndef _CLT_RENAME_FILES_H_
#define _CLT_RENAME_FILES_H_

#include "CmdLineBaseTask.h"

// in read only mode this cmd just checks if renaming is even possible
class CLTRenameFiles : public CmdLineBaseTask
{
public:
	CLTRenameFiles(const FvaConfiguration& cfg);
	virtual ~CLTRenameFiles()		{ LOG_QDEB << "cmd deleted, dir:" << m_folder; }
	virtual FVA_EXIT_CODE execute(const CLTContext& context);
	static QString Name()			{	return "CLTRenameFiles";					}
	virtual bool supportReadOnly()	{	return true;								}

protected:

	bool checkIfParentFileExist(const QFileInfo& fileToCheck, QDateTime& renameDateTime, const QDateTime& prevRenameDateTime);

	void fillRenameDateTimeFromLastModifiedIfValid(const QDir& dir, const QFileInfo& info, QDateTime& renameDateTime);

private:

	/*!
	 * file pates to file name
	 */
	QMap<QString, QString>			m_uniqueFileNames;

	/*!
	 * \brief shall be renamed video files using the file modification time if they do not have taken time set
	 */
	bool							m_renameVideoByModifTime;

	/*!
	 * \brief shall be renamed pictures files using the file modification time if they do not have exif taken time set
	 */
	bool							m_renamePicsByModifTime;

	/*!
	 * \brief formatting context to use formatting options from
	 */
	FvaFmtContext					m_fmtctx;
}; 

#endif // _CLT_RENAME_FILES_H_