/*!
* \file CLTCreateDirStructByFileNames.h
* \copyright Copyright 2021 FVA Software. All rights reserved. This file is released under the XXX License.
* \author Dima Nikulin.
* \version 0.29
* \date  2014-2021
*/
#ifndef _CMD_CREATE_DIR_STRUCTURE_BY_FILE_NAMES_H_
#define _CMD_CREATE_DIR_STRUCTURE_BY_FILE_NAMES_H

#include "CmdLineBaseTask.h"

/*!
* \brief CLTCreateDirStructByFileNames is child of CmdLineBaseTask and 
* it creates folder structure by year and days folders, using file names in input folder
* 
* we assume here only one-level structure as input
* INPUT_DIR_NAME
*		|
*		--FILE_NAME_1
*		|
*		--FILE_NAME_2
*		|
*		--FILE_NAME_3
*
* as output we will have there  3-level structure INPUT_DIR->YAER->DAY like
* INPUT_DIR_NAME
*		| 2021
*		-------2021.12.30
*				|
*				--FILE_NAME_1
*				|
*				--FILE_NAME_2
*		|
*		-------2021.12.31
*				|
*				--FILE_NAME_1
*				|
*				--FILE_NAME_2
*		| 2022
*		-------2022.01.30
*				|
*				--FILE_NAME_1
*				|
*				--FILE_NAME_2
*		|
*		-------2021.01.31
*				|
*				--FILE_NAME_1
*				|
*				--FILE_NAME_2
*
*/
class CLTCreateDirStructByFileNames : public CmdLineBaseTask
{
public:
	virtual ~CLTCreateDirStructByFileNames() { LOG_DEB << "cmd deleted, dir:" << m_folder; }
	virtual FVA_EXIT_CODE execute(const CLTContext& context);
	/*!
	* \brief returns command name
	* \return returns command name as a string
	*/
	static QString Name(){ return "CLTCreateDirStructByFileNames"; }
	virtual bool supportReadOnly() { return true; }
};


#endif // _CMD_CREATE_DIR_STRUCTURE_BY_FILE_NAMES_H