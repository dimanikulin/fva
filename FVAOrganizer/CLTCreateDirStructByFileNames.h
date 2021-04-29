#ifndef _CMD_CREATE_DIR_STRUCTURE_BY_FILE_NAMES_H_
#define _CMD_CREATE_DIR_STRUCTURE_BY_FILE_NAMES_H

#include "CmdLineBaseTask.h"

/*!
* \brief it creates folder structure by days folders, using file names in input folder
*/
class CLTCreateDirStructByFileNames : public CmdLineBaseTask
{
public:
	virtual ~CLTCreateDirStructByFileNames() { LOG_QDEB << "cmd deleted, dir:" << m_folder; }
	virtual FVA_EXIT_CODE execute(const CLTContext& context, const FvaConfiguration& cfg);
	static QString Name(){ return "CLTCreateDirStructByFileNames"; }
	virtual bool supportReadOnly() { return true; }
};


#endif // _CMD_CREATE_DIR_STRUCTURE_BY_FILE_NAMES_H