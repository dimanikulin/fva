#ifndef _CMD_JSON_CREATE_DIR_STRUCTURE_BY_FILE_NAMES_H_
#define _CMD_JSON_CREATE_DIR_STRUCTURE_BY_FILE_NAMES_H_

#include "CmdLineBaseTask.h"

/*!
* \brief it creates folder structure by days folders, using file names in input folder - old impl
*/
class CLTJSONCreateDirStructByFileNames : public CmdLineBaseTask
{
public:
	CLTJSONCreateDirStructByFileNames(const QString& dir_, bool readOnly_ = false, const QString& custom_ = "")
		:CmdLineBaseTask(dir_, readOnly_, custom_)
	{
		LOG_QDEB << "cmd created,dir:" << dir_ << ",RO=" << (readOnly_ ? "yes" : "no") << ",SRO=" << (supportReadOnly() ? "yes" : "no");
	}
	virtual ~CLTJSONCreateDirStructByFileNames() { LOG_QDEB << "cmd deleted, dir:" << m_folder; }
	virtual FVA_EXIT_CODE execute();
	static QString Name(){ return "CLTJSONCreateDirStructByFileNames"; }
	virtual bool supportReadOnly() { return true; }
};


#endif // _CMD_JSON_CREATE_DIR_STRUCTURE_BY_FILE_NAMES_H_