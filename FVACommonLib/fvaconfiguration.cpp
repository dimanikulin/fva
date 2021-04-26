#include "fvaconfiguration.h"

FVA_EXIT_CODE FvaConfiguration::load(const QString& path)
{
	FVADescriptionFile fvaFileCsv;

	FVA_EXIT_CODE res = fvaFileCsv.load(path, m_cfgtitles, m_cfgItems);
	RET_RES_IF_RES_IS_ERROR

	// ID,Name,Value
	int columnName = FVADescriptionFile::getColumnIdByName(m_cfgtitles, "Name");
	if (-1 == columnName)
		return FVA_ERROR_CANT_FIND_MANDATORY_FIELDS;

	int columnValue = FVADescriptionFile::getColumnIdByName(m_cfgtitles, "Value");
	if (-1 == columnValue)
		return FVA_ERROR_CANT_FIND_MANDATORY_FIELDS;

	return FVA_NO_ERROR;
}
FVA_EXIT_CODE FvaConfiguration::save(const QString& path)
{
	FVADescriptionFile fvaFileCsv;

	return fvaFileCsv.save(path, m_cfgtitles, m_cfgItems);
}