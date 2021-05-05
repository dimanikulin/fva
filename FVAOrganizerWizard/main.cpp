#include "fvaorganizerwizard.h"
#include <QtWidgets/QApplication>
#include <QtCore/qtextcodec>

#include "fvalogger.inl"

LOGGER_GLOBAL

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	FvaConfiguration cfg;
	FVA_EXIT_CODE res = cfg.load(QCoreApplication::applicationDirPath() + "/fvaParams.csv");
	RET_RES_IF_RES_IS_ERROR

	LOG_INIT("#logs/orgwizard.txt")
	FVAOrganizerWizard w;
	w.show();
	return a.exec();
}
