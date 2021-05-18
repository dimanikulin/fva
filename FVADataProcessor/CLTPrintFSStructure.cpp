/*!
* \file CLTPrintFSStructure.cpp
* \copyright Copyright 2021 FVA Software. All rights reserved. This file is released under the XXX License.
* \author Dima Nikulin.
* \version 0.29
* \date  2014-2021
*/
#include "CLTPrintFSStructure.h"

#include <QtCore/QCryptographicHash>

CLTPrintFSStructure::CLTPrintFSStructure(const FvaConfiguration& cfg)
{
	QString rootSWdir;
	FVA_EXIT_CODE res = cfg.getParamAsString("Common::RootDir", rootSWdir);
	RET_IF_RES_IS_ERROR

	m_file.setFileName(rootSWdir + "#logs/fsoutput.txt");
	m_file.open( QIODevice::WriteOnly );

	res = m_fmtctx.fillFmtContextFromCfg(cfg);
	RET_IF_RES_IS_ERROR
}

CLTPrintFSStructure::~CLTPrintFSStructure()
{
	m_file.close();
}

FVA_EXIT_CODE CLTPrintFSStructure::execute(const CLTContext& /*context*/)
{
	char		buffer [ 64* 1024 ];
	qint64		size = 0;
	QCryptographicHash hash( QCryptographicHash::Sha1 );
	QString		result;

	Q_FOREACH(QFileInfo info, m_dir.entryInfoList(QDir::NoDotAndDotDot | QDir::System | QDir::Hidden  | QDir::AllDirs | QDir::Files, QDir::DirsFirst))
	{
		if ( info.isDir() )
			continue;

		QFile file( info.absoluteFilePath() );

		if ( !file.open( QIODevice::ReadOnly ) ) 
		{
			LOG_QCRIT << "failed to open file:" << info.absoluteFilePath();
			continue;
		}
		while (!file.atEnd())
		{
			size	= file.read(buffer, 64 * 1024);
			if (size)
				hash.addData( buffer, size);
		}
		QFileInfo f(info);
		result =	info.filePath()										+ "," 
					+ hash.result().toBase64()						+ ","
					+ f.lastModified().toString(m_fmtctx.fvaFileName) + ","
					+ QString::number(info.size())					+ "\n";
		m_file.write(result.toLocal8Bit());
	}
	return FVA_NO_ERROR;
}
