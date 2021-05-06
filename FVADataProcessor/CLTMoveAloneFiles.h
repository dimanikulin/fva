#ifndef _CLT_MOVE_ALONE_FILES_H_
#define _CLT_MOVE_ALONE_FILES_H_

#include "CmdLineBaseTask.h"

/*!
* \brief it moves alone (or twins) files to parent folder, with adding their description into parent folder DESCRIPTION_FILE_NAME
*/
class CLTMoveAloneFiles : public CmdLineBaseTask
{
public:
	CLTMoveAloneFiles(const FvaConfiguration& cfg);
	virtual ~CLTMoveAloneFiles() { LOG_QDEB << "cmd deleted, dir:" << m_folder; }
	virtual FVA_EXIT_CODE execute(const CLTContext& context);
	static QString Name(){ return "CLTMoveAloneFiles"; }
	virtual bool supportReadOnly() { return true; }

private:
	/*!
	* min count of multimedia files in one folder allowed
	*/
	uint m_minCountSupportedFiles;
};
#endif // _CLT_MOVE_ALONE_FILES_H_