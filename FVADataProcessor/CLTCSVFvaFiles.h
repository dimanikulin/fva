#ifndef _CMD_CSV_FVA_FILES_H_
#define _CMD_CSV_FVA_FILES_H_

#include "CmdLineBaseTask.h"

/*!
 * \brief it creates CSV file with FVA information for files in a folder
 */
class CLTCSVFvaFile : public CmdLineBaseTask
{
public:
	/// CmdLineBaseTask interface
	CLTCSVFvaFile(const FvaConfiguration& cfg);
	virtual ~CLTCSVFvaFile()		{ LOG_QDEB << "cmd deleted, dir:" << m_folder; }
	virtual FVA_EXIT_CODE execute(const CLTContext& context);
	static QString Name()			{ return "CLTCSVFvaFile"; }
	virtual bool supportReadOnly()	{ return true; }

private: // data

	/*!
	* path to root fva sw folder
	*/
	QString							m_rootSWdir;
};

#endif // _CMD_CSV_FVA_FILES_H_