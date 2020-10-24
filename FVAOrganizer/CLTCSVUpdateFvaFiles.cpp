#include "CLTCSVUpdateFvaFiles.h"

#include "FVADescriptionFile.h"


CLTCSVUpdateFvaFiles::CLTCSVUpdateFvaFiles(const QString& dir_, bool readOnly_, const QString& custom_ )
:CmdLineBaseTask(dir_, readOnly_, custom_)
{
	LOG_QDEB << "cmd created,dir:" << dir_ << ",RO=" << (readOnly_ ? "yes" : "no") << ",SRO=" << (supportReadOnly() ? "yes" : "no");

	FVADescriptionFile fvaFileCsv;

	// firstly - try to get device if from fvaFile.csv as it has high priority 
	QStringList			titles;
	DESCRIPTIONS_MAP	decsItems;

	FVA_EXIT_CODE res = fvaFileCsv.load(FVA_DEFAULT_ROOT_DIR + "#data#/fvaFile.csv", titles, decsItems);
	RET_RES_IF_RES_IS_ERROR
}
FVA_EXIT_CODE CLTCSVUpdateFvaFiles::execute()
{
	return FVA_NO_ERROR;
}