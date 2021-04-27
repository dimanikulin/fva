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

QString FvaConfiguration::getParamAsString(const QString& paramName) const
{
	// ID,Name,Value
	int columnName = FVADescriptionFile::getColumnIdByName(m_cfgtitles, "Name");
	if (-1 == columnName)
		throw std::exception("FvaConfiguration::getParamAsString - FVA_ERROR_CANT_FIND_MANDATORY_FIELDS");

	int columnValue = FVADescriptionFile::getColumnIdByName(m_cfgtitles, "Value");
	if (-1 == columnValue)
		throw std::exception("FvaConfiguration::getParamAsString - FVA_ERROR_CANT_FIND_MANDATORY_FIELDS");

	for (DESCRIPTIONS_MAP::const_iterator it = m_cfgItems.begin(); it != m_cfgItems.end(); ++it)
	{
		QStringList list = it.value();
		QString paramName_ = list[columnName].toUpper();
		QString paramValue_ = list[columnValue];
		if (paramName.toUpper() == paramName_)
			return paramValue_;
	}
	throw std::exception("FvaConfiguration::getParamAsString - not implemented");
}

bool FvaConfiguration::getParamAsBoolean(const QString& paramName) const
{
	QString str = getParamAsString(paramName);
	if (str.toUpper() == "FALSE" || str.toUpper() == "NO")
		return false;
	if (str.toUpper() == "TRUE" || str.toUpper() == "YES")
		return true;
	throw std::exception("FvaConfiguration::getParamAsBoolean - not implemented");
}

uint FvaConfiguration::getParamAsUint(const QString& paramName) const
{
	QString str = getParamAsString(paramName);
	return str.toUInt();
}

FVA_EXIT_CODE FvaConfiguration::setParam(const QString& paramName, QString paramValue)
{
	// ID,Name,Value
	int columnName = FVADescriptionFile::getColumnIdByName(m_cfgtitles, "Name");
	if (-1 == columnName)
		throw std::exception("FvaConfiguration::getParamAsString - FVA_ERROR_CANT_FIND_MANDATORY_FIELDS");

	int columnValue = FVADescriptionFile::getColumnIdByName(m_cfgtitles, "Value");
	if (-1 == columnValue)
		throw std::exception("FvaConfiguration::getParamAsString - FVA_ERROR_CANT_FIND_MANDATORY_FIELDS");

	for (DESCRIPTIONS_MAP::const_iterator it = m_cfgItems.begin(); it != m_cfgItems.end(); ++it)
	{
		QStringList list = it.value();
		QString paramName_ = list[columnName].toUpper();
		if (paramName.toUpper() == paramName_)
		{
			list[columnValue] = paramValue;
			return FVA_NO_ERROR;
		}
	}
	return FVA_ERROR_CANT_SET_PARAM;
}
FVA_EXIT_CODE FvaConfiguration::setParam(const QString& paramName, bool paramValue)
{
	return setParam(paramName, QString(paramValue ? "TRUE" : "FALSE"));
}

FVA_EXIT_CODE FvaConfiguration::setParam(const QString& paramName, uint paramValue)
{
	return setParam(paramName, QString::number( paramValue ));
}