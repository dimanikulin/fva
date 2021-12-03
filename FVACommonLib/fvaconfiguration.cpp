/*!
* \file fvaconfiguration.cpp
* \copyright Copyright 2021 FVA Software. All rights reserved. This file is released under the XXX License.
* \author Dima Nikulin.
* \version 0.29
* \date  2014-2021
*/

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

FVA_EXIT_CODE FvaConfiguration::getParamAsString(const QString& paramName, QString& paramValue) const
{
	// ID,Name,Value
	int columnName = FVADescriptionFile::getColumnIdByName(m_cfgtitles, "Name");
	if (-1 == columnName)
		throw std::runtime_error("FvaConfiguration::getParamAsString - FVA_ERROR_CANT_FIND_MANDATORY_FIELDS");

	int columnValue = FVADescriptionFile::getColumnIdByName(m_cfgtitles, "Value");
	if (-1 == columnValue)
		throw std::runtime_error("FvaConfiguration::getParamAsString - FVA_ERROR_CANT_FIND_MANDATORY_FIELDS");

	for (DESCRIPTIONS_MAP::const_iterator it = m_cfgItems.begin(); it != m_cfgItems.end(); ++it)
	{
		QStringList list = it.value();
		QString paramName_ = list[columnName].toUpper();
		QString paramValue_ = list[columnValue];
		if (paramName.toUpper() == paramName_)
		{
			paramValue = paramValue_;
			return FVA_NO_ERROR;
		}
	}
	return FVA_ERROR_CANT_GET_PARAM;
}

FVA_EXIT_CODE FvaConfiguration::getParamAsBoolean(const QString& paramName, bool& paramValue) const
{
	QString str;
	FVA_EXIT_CODE res = getParamAsString(paramName, str);
	RET_RES_IF_RES_IS_ERROR
	if (str.toUpper() == "FALSE" || str.toUpper() == "NO")
	{
		paramValue = false;
		return FVA_NO_ERROR;
	}
	if (str.toUpper() == "TRUE" || str.toUpper() == "YES")
	{
		paramValue = true;
		return FVA_NO_ERROR;
	}
	return FVA_ERROR_CANT_GET_PARAM;
}

FVA_EXIT_CODE FvaConfiguration::getParamAsUint(const QString& paramName, uint& paramValue) const
{
	QString str;
	FVA_EXIT_CODE res = getParamAsString(paramName,str);
	RET_RES_IF_RES_IS_ERROR
	paramValue = str.toUInt();
	return FVA_NO_ERROR;
}

FVA_EXIT_CODE FvaConfiguration::setParam(const QString& paramName, QString paramValue)
{
	// ID,Name,Value
	int columnName = FVADescriptionFile::getColumnIdByName(m_cfgtitles, "Name");
	if (-1 == columnName)
		throw std::runtime_error("FvaConfiguration::getParamAsString - FVA_ERROR_CANT_FIND_MANDATORY_FIELDS");

	int columnValue = FVADescriptionFile::getColumnIdByName(m_cfgtitles, "Value");
	if (-1 == columnValue)
		throw std::runtime_error("FvaConfiguration::getParamAsString - FVA_ERROR_CANT_FIND_MANDATORY_FIELDS");

	for (DESCRIPTIONS_MAP::iterator it = m_cfgItems.begin(); it != m_cfgItems.end(); ++it)
	{
		QStringList list = it.value();
		QString paramName_ = list[columnName].toUpper();
		if (paramName.toUpper() == paramName_)
		{
			it.value()[columnValue] = paramValue;
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