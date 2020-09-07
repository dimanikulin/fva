#ifndef _CLT_JSON_MERGE_DIRS_H_
#define _CLT_JSON_MERGE_DIRS_H_

#include "CmdLineBaseTask.h"

/*!
 * \brief old impl of CLT_Folder_Merging
 */
class CLTJSONMergeDirs : public CmdLineBaseTask
{
public:
	CLTJSONMergeDirs(const QString& dir_, bool readOnly_ = false, const QString& custom_ = "")
		:CmdLineBaseTask(dir_, readOnly_, custom_)
	{
		LOG_QDEB << "cmd created,dir:" << dir_ << ",RO=" << (readOnly_ ? "yes" : "no") << ",SRO=" << (supportReadOnly() ? "yes" : "no");
	}
	virtual ~CLTJSONMergeDirs() { LOG_QDEB << "cmd deleted, dir:" << m_folder; }
	virtual FVA_EXIT_CODE execute();
	static QString Name(){ return "CLTJSONMergeDirs"; }
	virtual bool supportReadOnly() { return false; }
};
#endif // _CLT_JSON_MERGE_DIRS_H_