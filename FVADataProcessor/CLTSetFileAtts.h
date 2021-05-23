/*!
* \file CLTSetFileAtts.h
* \copyright Copyright 2021 FVA Software. All rights reserved. This file is released under the XXX License.
* \author Dima Nikulin.
* \version 0.29
* \date  2014-2021
*/
#ifndef _CLT_SET_FILE_ATTRS_H_
#define _CLT_SET_FILE_ATTRS_H_

#include "CmdLineBaseTask.h"

/*!
* \brief it sets up "read only" attribute for all files in input folder 
*/
class CLTSetFileAtts : public CmdLineBaseTask
{
public:
	virtual ~CLTSetFileAtts() { LOG_QDEB << "cmd deleted, dir:" << m_folder; }
	virtual FVA_EXIT_CODE execute(const CLTContext& context);
	/*!
	* \brief returns command name
	* \return returns command name as a string
	*/
	static QString Name(){ return "CLTSetFileAtts"; }
	virtual bool supportReadOnly() { return false; }
};
#endif // _CLT_SET_FILE_ATTRS_H_