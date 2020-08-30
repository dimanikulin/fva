#include "CLTPrintFSStructure.h"

#include "fvadefaultcfg.h"

#include <QtCore/QCryptographicHash>

CLTPrintFSStructure::CLTPrintFSStructure(const QString& dir_,bool readOnly_,const QString& custom_)
	:CmdLineBaseTask( dir_,readOnly_,custom_)
{
	qDebug()<<"["<<Name().toUpper()<<"]cmd created,dir:"<<dir_;
	m_file.setFileName(FVA_DEFAULT_ROOT_DIR + "fsoutput.txt");
	m_file.open( QIODevice::WriteOnly );
}

CLTPrintFSStructure::~CLTPrintFSStructure()
{
	m_file.close();
}

FVA_EXIT_CODE CLTPrintFSStructure::execute()
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
					+	hash.result().toBase64()						+ ","
					+	f.lastModified().toString("yyyy-MM-dd-hh-mm-ss")	+ ","
					+	QString::number(info.size())					+ "\n";
		m_file.write(result.toLocal8Bit());
	}
	return FVA_NO_ERROR;
}