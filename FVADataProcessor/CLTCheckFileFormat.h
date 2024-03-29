/*!
* \file CLTCheckFileFormat.h
* \copyright Copyright 2021 FVA Software. All rights reserved. This file is released under the XXX License.
* \author Dima Nikulin.
* \version 0.29
* \date  2014-2021
*/
#ifndef _CLT_CHECK_FILE_FORMAT_H_
#define _CLT_CHECK_FILE_FORMAT_H_

#include "CmdLineBaseTask.h"

/*!
* \brief CLTCheckFileFormat is child of CmdLineBaseTask and implements the next functions:
*
* 1. it checks for supported file format - returns FVA_ERROR_INCORRECT_FILE_FORMAT
* 2. TBD
*
*/
class CLTCheckFileFormat : public CmdLineBaseTask
{
public:
	virtual ~CLTCheckFileFormat() { LOG_DEB << "cmd deleted, dir:" << m_folder; }
	virtual FVA_EXIT_CODE execute(const CLTContext& context);
	/*!
	* \brief returns command name
	* \return returns command name as a string
	*/
	static QString Name(){ return "CLTCheckFileFormat"; }
	virtual bool supportReadOnly() { return false; }
};
#endif // _CLT_CHECK_FILE_FORMAT_H_