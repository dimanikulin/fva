#include "CLTXMLConvert.h"

#include <QtCore/QXmlStreamReader>


FVA_EXIT_CODE CLTXmlConvert::execute()
{
	std::auto_ptr< QFile > xmlFile(new QFile(m_folder/*it is path to file*/));

	if (!xmlFile->open(QIODevice::ReadOnly | QIODevice::Text))
	{
		LOG_QCRIT << "input file can not be open";
		return FVA_ERROR_CANT_OPEN_INPUT_FILE;
	}

	// Name,Place,People,Device,Description,Scaner
	// Name,Place,People,Device,Description,Scaner,Comment

	QXmlStreamReader xmlData(xmlFile.get());
	while (!xmlData.atEnd())
	{
		QXmlStreamReader::TokenType token = xmlData.readNext();
		if (token == QXmlStreamReader::StartDocument)
			continue;
		if (xmlData.name() != "foto")
			continue;
		QXmlStreamAttributes attributes = xmlData.attributes();

		QString name;
		if (attributes.hasAttribute("name"))
			name = attributes.value("name").toString();

		QString file;
		if (attributes.hasAttribute("file"))
			file = attributes.value("file").toString();

		QString comment;
		if (attributes.hasAttribute("comment"))
			comment = attributes.value("comment").toString();

		// Name,Place,People,Device,Description,Scaner,Comment
		if (!name.isEmpty())
			qDebug() << file << ",,,," << name << ",," << comment;
	}

	return FVA_NO_ERROR;
}