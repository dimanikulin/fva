/*!
* \file CLTCreateDirStructByDeviceName.h
* \copyright Copyright 2021 FVA Software. All rights reserved. This file is released under the XXX License.
* \author Dima Nikulin.
* \version 0.29
* \date  2014-2021
*/
#ifndef _CLT_CREATE_DIR_STRUCTURE_BY_DEVICE_NAMES_H_
#define _CLT_CREATE_DIR_STRUCTURE_BY_DEVICE_NAMES_H_

#include "CmdLineBaseTask.h"

/*!
 * \brief CLTCreateDirStructByFileNames is child of CmdLineBaseTask and 
 * it creates folder structure using device names in input folder
 */
class CLTCreateDirStructByDeviceName : public CmdLineBaseTask
{
public:
	virtual ~CLTCreateDirStructByDeviceName()	{ LOG_QDEB << "cmd deleted, dir:" << m_folder;	}
	virtual FVA_EXIT_CODE execute(const CLTContext& context);
	/*!
	* \brief returns command name
	* \return returns command name as a string
	*/
	static QString Name()			{	return "CLTCreateDirStructByDeviceName";				}
	virtual bool supportReadOnly()	{	return false;											}

}; 

#endif // _CLT_CREATE_DIR_STRUCTURE_BY_DEVICE_NAMES_H_