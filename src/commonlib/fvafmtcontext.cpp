/*!
* \file fvafmtcontext.cpp
* \copyright Copyright 2021 FVA Software. All rights reserved. This file is released under the XXX License.
* \author Dima Nikulin.
* \version 0.29
* \date  2014-2021
*/

#include "fvafmtcontext.h"
#include "fvalogger.inl"

FVA_EXIT_CODE FvaFmtContext::fillFmtContextFromCfg(const FvaConfiguration& cfg)
{
	FVA_EXIT_CODE res = cfg.getParamAsString("Format::fvaDirName", fvaDirName);
	if ( FVA_NO_ERROR != res )
	{
		LOG_CRIT << "FvaFmtContext::fillFmtContextFromCfg: Can't get parameter 'Format::fvaDirName' from configuration.";
		return res;
	}
	
	res = cfg.getParamAsString("Format::fvaDirNameYear", fvaDirNameYear);
	if ( FVA_NO_ERROR != res )
	{
		LOG_CRIT << "FvaFmtContext::fillFmtContextFromCfg: Can't get parameter 'Format::fvaDirNameYear' from configuration.";
		return res;
	}
	
	res = cfg.getParamAsString("Format::fvaFileName", fvaFileName);
	if ( FVA_NO_ERROR != res )
	{
		LOG_CRIT << "FvaFmtContext::fillFmtContextFromCfg: Can't get parameter 'Format::fvaFileName' from configuration.";
		return res;
	}

	res = cfg.getParamAsString("Format::exifDateTime", exifDateTime);
	if ( FVA_NO_ERROR != res )
	{
		LOG_CRIT << "FvaFmtContext::fillFmtContextFromCfg: Can't get parameter 'Format::exifDateTime' from configuration.";
		return res;
	}

	res = cfg.getParamAsString("Format::fileName1", fileName1);
	if ( FVA_NO_ERROR != res )
	{
		LOG_CRIT << "FvaFmtContext::fillFmtContextFromCfg: Can't get parameter 'Format::fileName1' from configuration.";
		return res;
	}

	res = cfg.getParamAsString("Format::fileName2", fileName2);
	if ( FVA_NO_ERROR != res )
	{
		LOG_CRIT << "FvaFmtContext::fillFmtContextFromCfg: Can't get parameter 'Format::fileName2' from configuration.";
		return res;
	}

	res = cfg.getParamAsString("Format::riffDateTime1", riffDateTime1);
	if ( FVA_NO_ERROR != res )
	{
		LOG_CRIT << "FvaFmtContext::fillFmtContextFromCfg: Can't get parameter 'Format::riffDateTime1' from configuration.";
		return res;
	}

	if ( FVA_NO_ERROR != cfg.getParamAsString("Format::riffDateTime2", riffDateTime2))
	{
		LOG_CRIT << "FvaFmtContext::fillFmtContextFromCfg: Can't get parameter 'Format::riffDateTime2' from configuration.";
		return res;
	}
	return FVA_NO_ERROR;
}
