#ifndef _CLT_AUTO_CHECKS_3_H_
#define _CLT_AUTO_CHECKS_3_H

#include "CmdLineBaseTask.h"

#include "data/fvafile.h"
#include "data/fvadevice.h"

class CLTAutoChecks3 : public CmdLineBaseTask
{
public:
	CLTAutoChecks3();
	virtual ~CLTAutoChecks3();
	virtual FVA_EXIT_CODE execute(const CLTContext& context, const FvaConfiguration& cfg);
	static QString Name(){ return "CLTAutoChecks3"; }
	virtual bool supportReadOnly() { return true; }

private:

	/*!
	* issues found, divided by type (in string format)
	*/
	QList<QString>				m_Issues;

	/*!
	* global device map
	*/
	DEVICE_MAP						m_deviceMap;

	/*!
	* global fva file information
	*/
	FVA_FILE_INFO_MAP				m_fvaFileInfo;

	/*!
	* copy of global fva file information
	*/
	FVA_FILE_INFO_MAP				m_fvaFileInfoC;

};
#endif // _CLT_AUTO_CHECKS_3_H