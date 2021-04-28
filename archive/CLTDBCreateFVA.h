#ifndef _CLT_DB_CREATE_FVA_H_
#define _CLT_DB_CREATE_FVA_H_

#include "CLTDBBase.h"

/*!
* \brief it creates the fva insert records to add into FVA DB for each file in this folder
*/
class CLTDBCreateFVA : public CLTDBBase
{
public:
	CLTDBCreateFVA(const QString& dir_, bool readOnly_ = false, const QString& custom_ = "")
		:CLTDBBase(dir_, readOnly_, custom_)
	{
		LOG_QDEB << "cmd created,dir:" << dir_ << ",RO=" << (readOnly_ ? "yes" : "no") << ",SRO=" << (supportReadOnly() ? "yes" : "no");
	}
	virtual ~CLTDBCreateFVA();
	virtual FVA_EXIT_CODE execute();
	static QString Name(){ return "CLTDBCreateFVA"; }

private: // data

	/*!
	* moved file descriptions
	*/
	QVector<QString>		m_movedFiles;

};
#endif // _CLT_DB_CREATE_FVA_H_