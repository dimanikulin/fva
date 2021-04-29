#ifndef _CLT_GET_FVA_DIR_TYPE_H_
#define _CLT_GET_FVA_DIR_TYPE_H_

#include "CmdLineBaseTask.h"

/*!
 * \brief it returns if input foder type is FVA_1_EVENT_1_DAY, FVA_1_EVENT_FEW_DAYS, FVA_FEW_EVENTS_1_DAY or FVA_1_EVENT_FEW_DAYS
 */
class CLTGetFvaDirType : public CmdLineBaseTask
{
public:
	virtual ~CLTGetFvaDirType() { LOG_QDEB << "cmd deleted, dir:" << m_folder; }
	virtual FVA_EXIT_CODE execute(const CLTContext& context, const FvaConfiguration& cfg);
	static QString Name(){ return "CLTGetFvaDirType"; }
	virtual bool supportReadOnly() { return true; }
};
#endif // _CLT_GET_FVA_DIR_TYPE_H_