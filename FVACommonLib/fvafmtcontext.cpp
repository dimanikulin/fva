/*!
* \file fvafmtcontext.cpp
* \copyright Copyright 2021 FVA Software. All rights reserved. This file is released under the XXX License.
* \author Dima Nikulin.
* \version 0.29
* \date  2014-2021
*/

#include "fvafmtcontext.h"

FVA_EXIT_CODE FvaFmtContext::fillFmtContextFromCfg(const FvaConfiguration& cfg)
{
	FVA_EXIT_CODE res = cfg.getParamAsString("Format::fvaDirName", fvaDirName);
	RET_RES_IF_RES_IS_ERROR

	res = cfg.getParamAsString("Format::fvaDirNameYear", fvaDirNameYear);
	RET_RES_IF_RES_IS_ERROR

	res = cfg.getParamAsString("Format::fvaFileName", fvaFileName);
	RET_RES_IF_RES_IS_ERROR

	res = cfg.getParamAsString("Format::exifDateTime", exifDateTime);
	RET_RES_IF_RES_IS_ERROR

	res = cfg.getParamAsString("Format::fileName1", fileName1);
	RET_RES_IF_RES_IS_ERROR

	res = cfg.getParamAsString("Format::fileName2", fileName2);
	RET_RES_IF_RES_IS_ERROR

	res = cfg.getParamAsString("Format::riffDateTime1", riffDateTime1);
	RET_RES_IF_RES_IS_ERROR

	return cfg.getParamAsString("Format::riffDateTime2", riffDateTime2);	
}
