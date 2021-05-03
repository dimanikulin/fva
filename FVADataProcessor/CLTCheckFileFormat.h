#ifndef _CLT_CHECK_FILE_FORMAT_H_
#define _CLT_CHECK_FILE_FORMAT_H_

#include "CmdLineBaseTask.h"

class CLTCheckFileFormat : public CmdLineBaseTask
{
public:
	virtual ~CLTCheckFileFormat() { LOG_QDEB << "cmd deleted, dir:" << m_folder; }
	virtual FVA_EXIT_CODE execute(const CLTContext& context);
	static QString Name(){ return "CLTCheckFileFormat"; }
	virtual bool supportReadOnly() { return false; }
};
#endif // _CLT_CHECK_FILE_FORMAT_H_