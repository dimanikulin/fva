#ifndef _CLT_AUTO_CHECKS_2_H_
#define _CLT_AUTO_CHECKS_2_H

#include "CmdLineBaseTask.h"
#include "fvafmtcontext.h"

class CLTAutoChecks2 : public CmdLineBaseTask
{
public:
	CLTAutoChecks2(const FvaConfiguration& cfg);
	virtual ~CLTAutoChecks2();
	virtual FVA_EXIT_CODE execute(const CLTContext& context);
	static QString Name(){return "CLTAutoChecks2";}
	virtual bool supportReadOnly() {return true;}

private: // data

	/*!
	 * count of files in each subfolder
	 */
	QMap<QString, unsigned int>		m_fileCount;

	/*!
	 * issues found, divided by type (in string format)
	 */	
	QList<QString>					m_Issues;

	/*!
	 * file names to file pathes
	 */
	QMap<QString, QString>			m_uniqueFileNames;

	/*!
	 * path to root fva sw folder
	 */
	QString							m_rootSWdir;

	/*!
	 * min count of multimedia files in one folder allowed
	 */
	uint							m_minCountSupportedFiles;

	/*!
	* \brief formatting context to use formatting options from
	*/
	FvaFmtContext					m_fmtctx;
};
#endif // _CLT_AUTO_CHECKS_2_H