#ifndef _CLT_FIX_EMPTY_DATE_TIME_H_
#define _CLT_FIX_EMPTY_DATE_TIME_H_

#include "CmdLineBaseTask.h"

class CLTFixEmptyDateTime : public CmdLineBaseTask
{
public:
	CLTFixEmptyDateTime(const FvaConfiguration& cfg);
	virtual ~CLTFixEmptyDateTime() { LOG_QDEB << "cmd deleted, dir:" << m_folder; }
	virtual FVA_EXIT_CODE execute(const CLTContext& context);
	static QString Name(){ return "CLTFixEmptyDateTime"; }
	virtual bool supportReadOnly() { return false; }

private:
	/*!
	 * \brief formatting context to use formatting options from
	 */
	FvaFmtContext	m_fmtctx;

	/*!
	 * \brief shall be fix pictures files using the file modification time if they do not have exif taken time set
	 */
	bool			m_fixPicsByModifTime;
};
#endif // _CLT_FIX_EMPTY_DATE_TIME_H_