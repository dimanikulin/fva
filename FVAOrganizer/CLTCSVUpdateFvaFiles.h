#ifndef _CMD_CSV_FVA_FILES_H_
#define _CMD_CSV_FVA_FILES_H_

#include "CmdLineBaseTask.h"

/*!
 * \brief it creates CSV file with FVA information for files in a folder
 */
class CLTCSVFvaFile : public CmdLineBaseTask
{
public:
	/// CmdLineBaseTask interface
	CLTCSVFvaFile(const QString& dir_, bool readOnly_ = false, const QString& custom_ = "")
		:CmdLineBaseTask(dir_, readOnly_, custom_)
	{
		LOG_QDEB << "cmd created,dir:" << dir_ << ",RO=" << (readOnly_ ? "yes" : "no") << ",SRO=" << (supportReadOnly() ? "yes" : "no");
	}
	virtual ~CLTCSVFvaFile()		{ LOG_QDEB << "cmd deleted, dir:" << m_folder; }
	virtual FVA_EXIT_CODE execute();
	static QString Name()			{ return "CLTCSVFvaFile"; }
	virtual bool supportReadOnly()	{ return true; }
};

#endif // _CMD_CSV_FVA_FILES_H_