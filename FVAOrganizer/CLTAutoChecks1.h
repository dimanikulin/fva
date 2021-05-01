#ifndef _CLT_AUTO_CHECKS_1_H_
#define _CLT_AUTO_CHECKS_1_H

#include "CmdLineBaseTask.h"

#include <QtCore/QCoreApplication>

class CLTAutoChecks1 : public CmdLineBaseTask
{
public:
	CLTAutoChecks1(const FvaConfiguration& cfg);
	virtual ~CLTAutoChecks1() { LOG_QDEB << "cmd deleted, dir:" << m_folder; }
	virtual FVA_EXIT_CODE execute(const CLTContext& context);
	static QString Name(){return "CLTAutoChecks1";}
	virtual bool supportReadOnly() {return true;}

private: 
	/*!
	 * \brief shall be renamed video files using the file modification time if they do not have taken time set
	 */
	bool m_renameVideoByModifTime;
};
#endif // _CLT_AUTO_CHECKS_2_H