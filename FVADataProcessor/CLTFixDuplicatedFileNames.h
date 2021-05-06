#ifndef _CLT_FIX_DUPLICATED_FILE_NAMES_H_
#define _CLT_FIX_DUPLICATED_FILE_NAMES_H_

#include "CmdLineBaseTask.h"

/*!
 * \brief it renames files in input folder if it finds the files with the same name in output folder
 */
class CLTFixDuplicatedFileNames : public CmdLineBaseTask
{
public:
	CLTFixDuplicatedFileNames(const FvaConfiguration& cfg);
	virtual ~CLTFixDuplicatedFileNames(){ LOG_QDEB << "cmd deleted, dir:" << m_folder;		}
	virtual FVA_EXIT_CODE execute(const CLTContext& context);
	static QString Name()				{	return "CLTFixDuplicatedFileNames";				}
	virtual bool supportReadOnly()		{	return false;									}

private: // data

	/*!
	* path to root fva sw folder
	*/
	QString							m_rootSWdir;
}; 

#endif // _CLT_FIX_DUPLICATED_FILE_NAMES_H_