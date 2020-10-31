#ifndef _CMD_CSV_UPDATE_FVA_FILES_H_
#define _CMD_CSV_UPDATE_FVA_FILES_H_

#include "CmdLineBaseTask.h"
#include "FVADescriptionFile.h"

/*!
* \brief it updates "fvaFile.csv" with placeId, EventId, Tags and ReasonPeople that it gets from "fvaFolder.csv"
*/
class CLTCSVUpdateFvaFiles : public CmdLineBaseTask
{
public:
	/// CmdLineBaseTask interface
	CLTCSVUpdateFvaFiles(const QString& dir_, bool readOnly_ = false, const QString& custom_ = "");
	virtual ~CLTCSVUpdateFvaFiles();
	virtual FVA_EXIT_CODE execute();
	static QString Name()			{ return "CLTCSVUpdateFvaFiles"; }
	virtual bool supportReadOnly()	{ return true; }

private:

	QStringList			m_fileTitles;
	DESCRIPTIONS_MAP	m_fileDecsItems;

	QStringList			m_dirTitles;
	DESCRIPTIONS_MAP	m_dirDecsItems;

	QMap<QString, int>	m_fileNameToID;

	QMap<QString, int>	m_dirNameToID;
};

#endif // _CMD_CSV_UPDATE_FVA_FILES_H_