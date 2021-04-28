#include "CLTJSONConvertDirDesc.h"

FVA_EXIT_CODE CLTJSONConvertDirDesc::execute()
{
	/*QString descFolderPath = m_folder + "/" + FVA_DIR_DESCRIPTION_FILE_NAME;
	if (!m_dir.exists(descFolderPath))
	{
		LOG_QWARN << "description does not exist in:" << m_folder;
		return FVA_NO_ERROR;
	}
	QFile file(descFolderPath);
	if (!file.open(QIODevice::ReadOnly))
	{
		LOG_QCRIT << "can not find old description in:" << m_folder;
		return FVA_ERROR_CANT_OPEN_DIR_DESC;
	}

	QTextStream readStream(&file);

	// readStream.setCodec("IBM866");
	QString jsonData = readStream.readAll();
	jsonData.remove("{");
	jsonData.remove("}");
	jsonData = "{" + jsonData + "}";
	file.close();

	/*bool res = false;
	QVariantMap result = QtJson::Json::parse ( jsonData, res ).toMap();

	if ( res && !result["event"].toString().isEmpty() )
	LOG_QWARN << "dir:" << m_folder << ",event:" << result["event"].toString();

	if ( !m_readOnly )
	{
	if ( !m_dir.rename( descFolderPath , descFolderPath + "_old" ) )
	{
	LOG_QCRIT << "can not rename description in:" << m_folder;
	return FVA_ERROR_CANT_RENAME_DIR_DESC;
	}
	QString error;
	FVA_EXIT_CODE res = fvaCreateFolderDescription( descFolderPath, result, error );
	if ( FVA_NO_ERROR != res )
	{
	LOG_QCRIT << error;
	return res;
	}

	LOG_QWARN << "updated folder description:" << descFolderPath;
	}*/
	return FVA_NO_ERROR;
}
