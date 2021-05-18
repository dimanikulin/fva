/*!
* \file CLTPrintFSStructure.h
* \copyright Copyright 2021 FVA Software. All rights reserved. This file is released under the XXX License.
* \author Dima Nikulin.
* \version 0.29
* \date  2014-2021
*/
#ifndef _CLT_PRINT_FS_STRUCTURE_H_
#define _CLT_PRINT_FS_STRUCTURE_H_


#include "CmdLineBaseTask.h"

/*!
 * \brief it prints file system structure with adding info to compare with fs structure on remote PC 
 */
class CLTPrintFSStructure : public CmdLineBaseTask
{
public:
	CLTPrintFSStructure(const FvaConfiguration& cfg);
	virtual ~CLTPrintFSStructure();
	virtual FVA_EXIT_CODE execute(const CLTContext& context);
	static QString Name(){return "CLTPrintFSStructure";}
	virtual bool supportReadOnly () {return false;}

private: // data

	
	/*!
	 * \brief file to output the file stucture info into
	 */
	QFile			m_file;

	/*!
	 * \brief formatting context to use formatting options from
	 */
	FvaFmtContext	m_fmtctx;
};

#endif // _CLT_PRINT_FS_STRUCTURE_H_
