/*!
* \file CLTCheckLocation.h
* \copyright Copyright 2021 FVA Software. All rights reserved. This file is released under the XXX License.
* \author Dima Nikulin.
* \version 0.29
* \date  2014-2021
*/
#ifndef _CLT_CHECK_LOCATION_H_
#define _CLT_CHECK_LOCATION_H_

#include "CmdLineBaseTask.h"

/*!
* \brief CLTCheckLocation is child of CmdLineBaseTask and implements the next functions:
*
* 1. it checks for existing location - returns FVA_ERROR_NO_EXIF_LOCATION if it found empty location and if mode is read only
* 2. if mode is not read only and it found empty location it creates file FVA_ERROR_NO_EXIF_LOCATION.csv and prints all the file pathes with no location 
*/
class CLTCheckLocation : public CmdLineBaseTask
{
public:
	virtual ~CLTCheckLocation(); 
	virtual FVA_EXIT_CODE execute(const CLTContext& context);

	/*!
	* \brief returns command name
	* \return returns command name as a string
	*/
	static QString Name(){ return "CLTCheckLocation"; }
	virtual bool supportReadOnly() { return true; }

private:
	/*!
	* issues found
	*/
	QList<QString>					m_Issues;
};
#endif // _CLT_CHECK_LOCATION_H_