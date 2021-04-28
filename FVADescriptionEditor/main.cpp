#include "fvadescriptioneditor.h"
#include "fvadescriptionfile.h"
#include "fvacommonlib.h"
#include "fvadefaultcfg.h"

#include <QtWidgets/QApplication>
#include <QtCore/QDir>
#include <QtWidgets/QMessageBox>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	
	bool folderMode = true;

	FVADescriptionEditor w( folderMode, nullptr);
	w.show();
	return a.exec();
}