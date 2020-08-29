#ifndef _CLT_FIX_DUPLICATED_FILE_NAMES_H_
#define _CLT_FIX_DUPLICATED_FILE_NAMES_H_

#include "CmdLineBaseTask.h"

/*!
 * \brief it renames files in input folder if it finds the files with the same name in output folder
 */
class CLTFixDuplicatedFileNames : public CmdLineBaseTask
{
public:
	/// CmdLineBaseTask interface
	CLTFixDuplicatedFileNames(const QString& dir_, bool readOnly_ = false, const QString& custom_ = "")
	:CmdLineBaseTask(dir_, readOnly_, custom_)
										{	LOG_QDEB << "cmd created, dir:" << dir_;		}
	virtual ~CLTFixDuplicatedFileNames(){ LOG_QDEB << "cmd deleted, dir:" << m_folder;		}
	virtual FVA_EXIT_CODE execute();
	static QString Name()				{	return "CLTFixDuplicatedFileNames";				}
	virtual bool supportReadOnly()		{	return false;									}
}; 

#endif // _CLT_FIX_DUPLICATED_FILE_NAMES_H_