#ifndef _CLT_AUTO_CHECKS_3_H_
#define _CLT_AUTO_CHECKS_3_H

#include "CmdLineBaseTask.h"
#include "fvacommoncsv.h"

class CLTAutoChecks3 : public CmdLineBaseTask
{
public:
	CLTAutoChecks3(const QString& dir_, bool readOnly_ = false, const QString& custom_ = "");
	virtual ~CLTAutoChecks3();
	virtual FVA_EXIT_CODE execute();
	static QString Name(){ return "CLTAutoChecks3"; }
	virtual bool supportReadOnly() { return true; }

private:

	/*!
	* issues found, divided by type (in string format)
	*/
	QVector<QString>				m_Issues;

	/*!
	* global device map
	*/
	DEVICE_MAP						m_deviceMap;

	/*!
	* global fva file information
	*/
	FVA_FILE_INFO_MAP				m_fvaFileInfo;

};
#endif // _CLT_AUTO_CHECKS_3_H