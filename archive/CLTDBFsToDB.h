#ifndef _CMD_DB_FS_TO_DB_H_
#define _CMD_DB_FS_TO_DB_H_

#include "CmdLineBaseTask.h"

#include "CLTDBBase.h"

class CLTDBFsToDB : public CLTDBBase
{
public:
	CLTDBFsToDB(const QString& dir_, bool readOnly_ = false, const QString& custom_ = "")
		: CLTDBBase(dir_, readOnly_, custom_), m_skippedFiles(0)
	{
		LOG_QDEB << "cmd created,dir:" << dir_ << ",RO=" << (readOnly_ ? "yes" : "no") << ",SRO=" << (supportReadOnly() ? "yes" : "no");
	}
	virtual ~CLTDBFsToDB();
	virtual FVA_EXIT_CODE execute();
	static QString Name(){return "CLTDBFsToDB";}
	
private: // data

	/*!
	 * skipped files number
	 */
	int						m_skippedFiles;
};


#endif // _CMD_DB_FS_TO_DB_H_