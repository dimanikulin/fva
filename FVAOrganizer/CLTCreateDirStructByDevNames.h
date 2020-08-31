#ifndef _CLT_CREATE_DIR_STRUCTURE_BY_DEVICE_NAMES_H_
#define _CLT_CREATE_DIR_STRUCTURE_BY_DEVICE_NAMES_H_

#include "CmdLineBaseTask.h"

/*!
 * \brief it creates folder structure using device names in input folder
 */
class CLTCreateDirStructByDeviceName : public CmdLineBaseTask
{
public:
	/// CmdLineBaseTask interface
	CLTCreateDirStructByDeviceName(const QString& dir_, bool readOnly_ = false, const QString& custom_ = "")
	:CmdLineBaseTask(dir_, readOnly_, custom_)
	{
		LOG_QDEB << "cmd created,dir:" << dir_ << ",RO=" << (readOnly_ ? "yes" : "no") << ",SRO=" << (supportReadOnly() ? "yes" : "no");

	}
	virtual ~CLTCreateDirStructByDeviceName()	{ LOG_QDEB << "cmd deleted, dir:" << m_folder; }
	virtual FVA_EXIT_CODE execute();
	static QString Name()			{	return "CLTCreateDirStructByDeviceName";						}
	virtual bool supportReadOnly()	{	return false;									}

}; 

#endif // _CLT_CREATE_DIR_STRUCTURE_BY_DEVICE_NAMES_H_