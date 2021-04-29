#ifndef _CLT_PRINT_FS_STRUCTURE_H_
#define _CLT_PRINT_FS_STRUCTURE_H_


#include "CmdLineBaseTask.h"

/*!
 * \brief it prints file system structure with adding info to compare with fs structure on remote PC 
 */
class CLTPrintFSStructure : public CmdLineBaseTask
{
public:
	CLTPrintFSStructure();
	virtual ~CLTPrintFSStructure();
	virtual FVA_EXIT_CODE execute(const CLTContext& context, const FvaConfiguration& cfg);
	static QString Name(){return "CLTPrintFSStructure";}
	virtual bool supportReadOnly () {return false;}

private: // data

	QFile m_file;
};

#endif // _CLT_PRINT_FS_STRUCTURE_H_
