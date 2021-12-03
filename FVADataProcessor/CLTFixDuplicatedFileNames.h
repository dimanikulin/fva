/*!
* \file CLTFixDuplicatedFileNames.h
* \copyright Copyright 2021 FVA Software. All rights reserved. This file is released under the XXX License.
* \author Dima Nikulin.
* \version 0.29
* \date  2014-2021
*/
#ifndef _CLT_FIX_DUPLICATED_FILE_NAMES_H_
#define _CLT_FIX_DUPLICATED_FILE_NAMES_H_

#include "../FVACommonLib/CmdLineBaseTask.h"

/*!
 * \brief CLTFixDuplicatedFileNames is child of CmdLineBaseTask and 
 * it renames files in input folder if it finds the files with the same name in output folder
 */
class CLTFixDuplicatedFileNames : public CmdLineBaseTask
{
public:
	CLTFixDuplicatedFileNames(const FvaConfiguration& cfg);
	virtual ~CLTFixDuplicatedFileNames(){ LOG_QDEB << "cmd deleted, dir:" << m_folder;		}
	virtual FVA_EXIT_CODE execute(const CLTContext& context);
	/*!
	* \brief returns command name
	* \return returns command name as a string
	*/
	static QString Name()				{	return "CLTFixDuplicatedFileNames";				}
	virtual bool supportReadOnly()		{	return false;									}

private: // data

	/*!
	* path to root fva sw folder
	*/
	QString							m_rootSWdir;
}; 

#endif // _CLT_FIX_DUPLICATED_FILE_NAMES_H_