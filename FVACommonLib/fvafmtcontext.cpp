#include "fvafmtcontext.h"

FVA_EXIT_CODE FvaFmtContext::fillFmtContextFromCfg(const FvaConfiguration& cfg)
{
	FVA_EXIT_CODE res = cfg.getParamAsString("Format::fvaDirName", fvaDirName);
	RET_RES_IF_RES_IS_ERROR

	res = cfg.getParamAsString("Format::fvaDirNameYear", fvaDirNameYear);
	RET_RES_IF_RES_IS_ERROR

	res = cfg.getParamAsString("Format::fvaFileName", fvaFileName);
	RET_RES_IF_RES_IS_ERROR

	return cfg.getParamAsString("Format::exifDateTime", exifDateTime);
}
