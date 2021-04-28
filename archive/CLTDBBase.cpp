#include "CLTDBBase.h"

void CLTDBBase::SaveSQL(const QString& fileToSaveIn)
{
	QFile fileNew(fileToSaveIn);
	if (!fileNew.open(QIODevice::Append | QIODevice::Text))
	{
		LOG_QCRIT << "can not create fva sql for " << m_folder;
		return;
	}
	QTextStream writeStream(&fileNew);
	writeStream.setCodec("UTF-8");
	writeStream << "BEGIN TRANSACTION;" << "\n";
	for (auto it = m_SQLs.begin(); it != m_SQLs.end(); ++it)
		writeStream << *it << "\n";
	writeStream << "COMMIT;";
	writeStream.flush();
	fileNew.close();
}

