#ifndef _CLT_CREATE_DIR_STRUCTURE_BY_DEVICE_NAMES_H_
#define _CLT_CREATE_DIR_STRUCTURE_BY_DEVICE_NAMES_H_

#include "CmdLineBaseTask.h"

/*!
 * \brief it creates folder structure using device names in input folder
 */
class CLTCreateDirStructByDeviceName : public CmdLineBaseTask
{
public:
	virtual ~CLTCreateDirStructByDeviceName()	{ LOG_QDEB << "cmd deleted, dir:" << m_folder; }
	virtual FVA_EXIT_CODE execute(const CLTContext& context);
	static QString Name()			{	return "CLTCreateDirStructByDeviceName";						}
	virtual bool supportReadOnly()	{	return false;									}

}; 

#endif // _CLT_CREATE_DIR_STRUCTURE_BY_DEVICE_NAMES_H_