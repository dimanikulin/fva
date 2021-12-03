/*!
* \file CLTCheckDeviceName.h
* \copyright Copyright 2021 FVA Software. All rights reserved. This file is released under the XXX License.
* \author Dima Nikulin.
* \version 0.29
* \date  2014-2021
*/
#ifndef _CLT_CHECK_DEVICE_NAME_H_
#define _CLT_CHECK_DEVICE_NAME_H_

#include "../FVACommonLib/CmdLineBaseTask.h"

/*!
* \brief CLTCheckDeviceName is child of CmdLineBaseTask and implements the next functions:
*
* 1. it checks for existing EXIF model and maker name - returns FVA_ERROR_EMPTY_DEVICE_NAME
* 2. it checks for uniquie device name as EXIF model + maker name - returns FVA_ERROR_NON_UNIQUE_DEVICE_NAME
*
*/
class CLTCheckDeviceName : public CmdLineBaseTask
{
public:
	virtual ~CLTCheckDeviceName() { LOG_QDEB << "cmd deleted, dir:" << m_folder; }
	virtual FVA_EXIT_CODE execute(const CLTContext& context);
	/*!
	* \brief returns command name
	* \return returns command name as a string
	*/
	static QString Name(){ return "CLTCheckDeviceName"; }
	virtual bool supportReadOnly() { return false; }
};
#endif // _CLT_CHECK_DEVICE_NAME_H_