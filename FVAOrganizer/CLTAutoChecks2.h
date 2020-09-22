#ifndef _CLT_AUTO_CHECKS_2_H_
#define _CLT_AUTO_CHECKS_2_H

#include "CmdLineBaseTask.h"

class CLTAutoChecks2 : public CmdLineBaseTask
{
public:
	CLTAutoChecks2(const QString& dir_, bool readOnly_ = false, const QString& custom_ = "")
		:CmdLineBaseTask(dir_, readOnly_, custom_)
	{
		LOG_QDEB << "cmd created,dir:" << dir_ << ",RO=" << (readOnly_ ? "yes" : "no") << ",SRO=" << (supportReadOnly() ? "yes" : "no");
	}
	virtual ~CLTAutoChecks2();
	virtual FVA_EXIT_CODE execute();
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
	QList<QString>				m_Issues;

	/*!
	 * file names to file pathes
	 */
	QMap<QString, QString>			m_uniqueFileNames;
};
#endif // _CLT_AUTO_CHECKS_2_H