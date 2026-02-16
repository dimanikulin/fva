/*!
* \file fvadescriptioneditor.cpp
* \copyright Copyright 2021 FVA Software. All rights reserved. This file is released under the XXX License.
* \author Dima Nikulin.
* \version 0.29
* \date  2014-2021
*/
#include "fvadescriptioneditor.h"
#include "FVADescriptionFile.h"
#include "fvapeoplelistdlg.h"
#include "fvacommonui.h"
#include "fvaconfiguration.h"
#include "fvalogger.inl"

#include <QtCore/QFileInfo>
#include <QListWidgetItem>
#include <QtCore/QDir>
#include <QtCore/QDateTime>
#include <QtCore/QProcess>

FVADescriptionEditor::FVADescriptionEditor(bool	forFolder, QWidget*	parent)
	:
	QMainWindow(parent),
	m_forFolder(forFolder)
{
	LOG_DEB << "construction";

	ui.setupUi(this);

	/*connect (ui.btnNext,SIGNAL(clicked()),this,SLOT(OnNextBtnPressed()) );
	connect (ui.btnPrev,SIGNAL(clicked()),this,SLOT(OnPrevBtnPressed()) );
	connect (ui.btnSave,SIGNAL(clicked()),this,SLOT(OnSaveBtnPressed()) );
	connect (ui.btnDict,SIGNAL(clicked()),this,SLOT(OnChangeDictPressed()));*/

	QIcon	icon = QIcon(QCoreApplication::applicationDirPath() + "/Icons/main.png");
	setWindowIcon(icon);

	if (m_forFolder)
	{
		ui.editDescription->setVisible(false);
		setFixedHeight(size().height());
		setFixedWidth(ui.groupBox->size().width() + 20);
		ui.btnNext->setVisible(false);
		ui.btnPrev->setVisible(false);
	}
	else
	{
		// TODO	
	}
        LOG_DEB << "before loading cfg";
	FvaConfiguration cfg;
	FVA_EXIT_CODE exitCode = cfg.load(QCoreApplication::applicationDirPath() + "/fvaParams.csv");
	IF_CLT_ERROR_SHOW_MSG_BOX_AND_RET("FVADescriptionEditor.load.cfg")

	QString rootSWdir;
	exitCode = cfg.getParamAsString("Common::RootDir", rootSWdir);
	IF_CLT_ERROR_SHOW_MSG_BOX_AND_RET("FVADescriptionEditor.get.param")

	LOG_DEB << "before fvaBuildPeopleTree 1";	
	exitCode =fvaBuildPeopleTree(this, ui.treePeopleWidget, rootSWdir);
	IF_CLT_ERROR_SHOW_MSG_BOX_AND_RET("FVADescriptionEditor.fvaBuildPeopleTree.ui.treePeopleWidget")

	LOG_DEB << "before fvaBuildPeopleTree 2";	
	exitCode =fvaBuildPeopleTree(this, ui.treePEventWidget, rootSWdir);
	IF_CLT_ERROR_SHOW_MSG_BOX_AND_RET("FVADescriptionEditor.fvaBuildPeopleTree.ui.treePEventWidget")

	exitCode = fvaBuildEventTree(this, ui.treeEventWidget, rootSWdir);
	IF_CLT_ERROR_SHOW_MSG_BOX_AND_RET("fvaBuildEventTree")

	LOG_DEB << "constructed";
}

FVADescriptionEditor::~FVADescriptionEditor()
{
	LOG_DEB << "destructed";
}

void FVADescriptionEditor::updateChecks(QTreeWidgetItem *item, int column)
{
	fvaUpdateChecks(item, column);
}

/*
void FVADescriptionEditor::updateDictionaryGUI()
{
	fvaBuildPeopleFilterTree(this, ui.treePeopleWidget, false,m_dictionaries);
	fvaBuildPeopleFilterTree(this, ui.treePEventWidget, false, m_dictionaries);

	fvaBuildFilterTree( this, 
						ui.treePlaceWidget, 
						m_dictionaries["placeTypes"].toList(), 
						m_dictionaries["places"].toList(),
						nullptr,
						nullptr);

	fvaBuildFilterTree( this, 
						ui.treeEventWidget, 
						m_dictionaries["eventTypes"].toList(), 
						m_dictionaries["events"].toList(),
						nullptr,
						nullptr);

	QVariantList vlist;
	FILL_COMB_FROM_DICT("devices",ui.cmbBoxDevice);
}

void FVADescriptionEditor::OnNextBtnPressed()
{
	saveCurrentDescription();
	if ( m_files.size() < m_indexOfFile + 1 )
		return;
	m_indexOfFile++;
	updateGuiForItem(m_files[m_indexOfFile]);
}
void FVADescriptionEditor::OnPrevBtnPressed()
{
	saveCurrentDescription();
	if ( 0 > m_indexOfFile - 1 )
		return;
	m_indexOfFile--;
	updateGuiForItem(m_files[m_indexOfFile]);
}
void FVADescriptionEditor::OnSaveBtnPressed()
{
	saveCurrentDescription();
	close();
}

void setCheckedItem(QTreeWidgetItem *item, QVector<unsigned int>& Ids)
{
	for (auto it = Ids.begin(); it != Ids.end(); ++it)
	{
		auto ID = item->data(1, 1).toUInt();
		auto Id = (*it);
		if(ID == Id)
			item->setCheckState(0, Qt::CheckState::Checked);
	}

	for (auto id = 0 ; id < item->childCount(); ++id)
		setCheckedItem(item->child(id), Ids);
}

void FVADescriptionEditor::updateGuiForItem( const QString& path )
{
	QFileInfo info( path );
	auto it = m_decsItems.find( info.fileName().toUpper() );
	if ( it == m_decsItems.end() )
		return;

	setWindowTitle(info.fileName());

	if (m_forFolder)
	{
		ui.editDescription->setVisible(false);
		setFixedHeight( size().height()	);
		setFixedWidth( ui.groupBox->size().width() + 20 );
		ui.btnNext->setVisible(false);
		ui.btnPrev->setVisible(false);
	}
	else
	{
		ui.treeEventWidget->setVisible(false);
		ui.treePEventWidget->setVisible(false);
		fvaShowImage( path, ui.lbFoto,"" );
	}
	QStringList list = it.value(); 

	int columnId = FVADescriptionFile::getColumnIdByName(m_titles,"oldName");
	if ( -1 == columnId )
		return;
	ui.editOldName->setText(list[columnId].remove("\t"));

	int decsColumnId = FVADescriptionFile::getColumnIdByName(m_titles,"Description");
	if ( -1 == decsColumnId )
		return;
	ui.editDescription->setText(list[decsColumnId].remove("\t"));

	columnId = FVADescriptionFile::getColumnIdByName(m_titles,"Comment");
	if ( -1 == columnId )
		return;
	ui.editComment->setText(list[columnId].remove("\t"));

	if (m_forFolder)
	{
		int eventID = list[decsColumnId].remove("\t").toInt();
		if (eventID)
			ui.editComment->setText( ui.editComment->text());
		else
			ui.editComment->setText( ui.editComment->text() +" " + list[decsColumnId].remove("\t") );
	}
	//////////////// DEVICE /////////////////////
	columnId = FVADescriptionFile::getColumnIdByName(m_titles,"Device");
	if ( -1 == columnId )
		return;
	QString selID = list[columnId].remove("\t");
	int index = ui.cmbBoxDevice->findData( selID );
	if ( index != -1 )
		ui.cmbBoxDevice->setCurrentIndex(index);
	else 
		ui.cmbBoxDevice->setCurrentIndex(0);
	
	//////// PLACE/////////////////////
	QVector<unsigned int> Ids;
	columnId = FVADescriptionFile::getColumnIdByName(m_titles,"Place");
	if ( -1 == columnId )
		return;
	selID = list[columnId].remove("\t");
	Ids.push_back(selID.toInt());
	if ( !selID.isEmpty() && !Ids.isEmpty())
	{
		for (auto idTop = 0; idTop < ui.treePlaceWidget->topLevelItemCount();++idTop )
			setCheckedItem(ui.treePlaceWidget->topLevelItem(idTop), Ids);
	}

	//////// PEOPLE/////////////////////
	columnId = FVADescriptionFile::getColumnIdByName(m_titles,"People");
	if ( -1 == columnId )
		return;
	QStringList peopleList = list[columnId].split(',');
	Ids.clear();
	for (auto idP = peopleList.begin(); idP != peopleList.end(); ++idP)
	{
		if ((*idP).toInt() != 0)
			Ids.push_back((*idP).toInt());
	}
	if (!Ids.isEmpty())
	{
		for (auto idTop = 0; idTop < ui.treePeopleWidget->topLevelItemCount();++idTop )
			setCheckedItem(ui.treePeopleWidget->topLevelItem(idTop), Ids);
	}
	//////// EVENT/////////////////////
	if (m_forFolder)
	{
		selID = list[decsColumnId].remove("\t");
		if (selID.toInt())
		{
			Ids.clear();
			Ids.push_back(selID.toInt());
			for (auto idTop = 0; idTop < ui.treeEventWidget->topLevelItemCount();++idTop )
				setCheckedItem(ui.treeEventWidget->topLevelItem(idTop), Ids);
		}
	}
	//////// PEOPLE LINKED/////////////////////
	if (m_forFolder)
	{
		columnId = FVADescriptionFile::getColumnIdByName(m_titles,"Scaner");
		if ( -1 == columnId )
			return;
		peopleList = list[columnId].split(',');
		Ids.clear();
		for (auto idP = peopleList.begin(); idP != peopleList.end(); ++idP)
		{
			if ((*idP).toInt() != 0)
				Ids.push_back((*idP).toInt());
		}
		if (!Ids.isEmpty())
		{
			for (auto idTop = 0; idTop < ui.treePEventWidget->topLevelItemCount();++idTop )
				setCheckedItem(ui.treePEventWidget->topLevelItem(idTop), Ids);
		}
	}
}

void FVADescriptionEditor::saveCurrentDescription()
{
	////////// PEOPLE /////////////////////
	QList<unsigned int> Ids;
	for (auto idTop = 0; idTop < ui.treePeopleWidget->topLevelItemCount();++idTop )
		fvaFindCheckedItem(ui.treePeopleWidget->topLevelItem(idTop), Ids);

	QString peopleIDs;
	for ( auto iP = Ids.begin(); iP != Ids.end(); ++iP )
	{
		if ( peopleIDs.isEmpty() )
			peopleIDs = QString::number (*iP);
		else
			peopleIDs += "," + QString::number (*iP);
	}
	QString place;
	Ids.clear();
	for (auto idTop = 0; idTop < ui.treePlaceWidget->topLevelItemCount();++idTop )
		fvaFindCheckedItem(ui.treePlaceWidget->topLevelItem(idTop), Ids);

	if (Ids.count() == 1)
		place = QString::number(Ids.at(0));

	if (m_forFolder)
	{
		QString device, event;
		int index; 
#define GET_SELECTED_FROM_COMBO_(str,combo) \
		index = combo->currentIndex(); \
		if ( 1 <= index ) \
		{\
			str = combo->itemData( index ).toString();\
		}
		GET_SELECTED_FROM_COMBO_(device,ui.cmbBoxDevice)

		////////// LINKED PEOPLE /////////////////////
		Ids.clear();
		for (auto idTop = 0; idTop < ui.treePEventWidget->topLevelItemCount();++idTop )
			fvaFindCheckedItem(ui.treePEventWidget->topLevelItem(idTop), Ids);

		QString linkedPeopleIDs;
		for ( auto iP = Ids.begin(); iP != Ids.end(); ++iP )
		{
			if ( linkedPeopleIDs.isEmpty() )
				linkedPeopleIDs = QString::number (*iP);
			else
				linkedPeopleIDs += "," + QString::number (*iP);
		}
		////////// EVENT /////////////////////
		Ids.clear();
		for (auto idTop = 0; idTop < ui.treeEventWidget->topLevelItemCount();++idTop )
			fvaFindCheckedItem(ui.treeEventWidget->topLevelItem(idTop), Ids);
		if (Ids.count() == 1)
			event = QString::number(Ids.at(0));

		QVariantMap content;
		content["deviceId"] = device;

		// TODO to clean up
		//QString jsonData = 
		//"{\"deviceId\":\""				+ device										+ "\",\n";
		if ( !ui.editComment->text().remove("\t").trimmed().isEmpty())
			//jsonData += "\"tags\":\""	+ ui.editComment->text().remove("\t").trimmed()	+ "\",\n";
			content["tags"] = ui.editComment->text().remove("\t").trimmed();
		if (!peopleIDs.isEmpty())
			// jsonData += "\"people\":\""	+ peopleIDs										+ "\",\n";
			content["people"] = peopleIDs;
		if (!place.isEmpty())
			// jsonData += "\"place\":\""	+ place											+ "\",\n";
			content["place"] = place;
		if (!event.isEmpty())
			// jsonData += "\"event\":\""	+ event											+ "\",\n";
			content["event"] = event;
		if (!linkedPeopleIDs.isEmpty())
			// jsonData += "\"reasonPeople\":\""	+ linkedPeopleIDs						+ "\",\n";
			content["reasonPeople"] = linkedPeopleIDs;
		if (!ui.editOldName->text().remove("\t").isEmpty())
			// jsonData += "\"linkedFolder\":\""	+ ui.editOldName->text().remove("\t")	+ "\"";
			content["linkedFolder"] = ui.editOldName->text().remove("\t");
		//jsonData += "}";
		
		QDir dir(m_folderPath);
		if ( dir.exists( dir.absolutePath() + "/" + FVA_DIR_DESCRIPTION_FILE_NAME ) )
		{
			QString newName = dir.absolutePath()
				+ "/" + FVA_DIR_DESCRIPTION_FILE_NAME 
				+ "_" + QDateTime::currentDateTime().toString( "yyyy-MM-dd-hh-mm-ss").toLatin1().data();
			QString oldName = dir.absolutePath()+ "/" + FVA_DIR_DESCRIPTION_FILE_NAME;
			if ( !dir.rename( oldName, newName ))
			{
				return;
			}
		}
		QString error;
		FVA_EXIT_CODE res = fvaCreateFolderDescription( m_folderPath + "/" + FVA_DIR_DESCRIPTION_FILE_NAME, content, error );
		RET_IF_RES_NO_ERROR
	}
	else
	{
		QFileInfo info( m_files[m_indexOfFile] );
		auto it = m_decsItems.find(info.fileName().toUpper() );
		if ( it == m_decsItems.end() )
			return;

		QStringList& list = it.value();
		list[FVADescriptionFile::getColumnIdByName(m_titles,"oldName")]		= ui.editOldName->text().remove("\t");
		list[FVADescriptionFile::getColumnIdByName(m_titles,"Description")] = ui.editDescription->text().remove("\t");
		list[FVADescriptionFile::getColumnIdByName(m_titles,"Comment")]		= ui.editComment->text().remove("\t");
		int index = -1;
#define GET_SELECTED_FROM_COMBO(col,combo) \
		index = combo->currentIndex(); \
		if ( 1 <= index ) \
		{\
			QString ID = combo->itemData( index ).toString();\
			list[FVADescriptionFile::getColumnIdByName(m_titles,col)] = ID;\
		}
		//GET_SELECTED_FROM_COMBO("place",ui.cmbBoxPlace)
		GET_SELECTED_FROM_COMBO("device",ui.cmbBoxDevice)
		//GET_SELECTED_FROM_COMBO("scaner",ui.cmbBoxScaner)

		list[FVADescriptionFile::getColumnIdByName(m_titles,"people")] = peopleIDs;

		QDir dir(m_files[m_indexOfFile]);
		dir.cdUp();
		if ( dir.exists( dir.absolutePath() + "/" + FVA_DESCRIPTION_FILE_NAME ) )
		{
			QString newName = dir.absolutePath()
				+ "/" + FVA_DESCRIPTION_FILE_NAME 
				+ "_" + QDateTime::currentDateTime().toString( "yyyy-MM-dd-hh-mm-ss").toLatin1().data();
			QString oldName = dir.absolutePath()+ "/" + FVA_DESCRIPTION_FILE_NAME;
			if ( !dir.rename( oldName, newName ))
			{
				return;
			}
		}
	
		//FVADescriptionFile desc;
		//FVA_EXIT_CODE res = desc.save( dir.absolutePath() + "/" + FVA_DESCRIPTION_FILE_NAME, 
			//							m_titles, 
				//						m_decsItems );
		// RET_IF_RES_NO_ERROR

	}
}

void FVADescriptionEditor::OnChangeDictPressed()
{
	QProcess myProcess(this);    
	myProcess.setProcessChannelMode(QProcess::MergedChannels);
	QStringList params;
	params.append(m_dictPath);
	myProcess.start("FVADictionaryEditor.exe", params);

	myProcess.waitForFinished( -1 );

	QString		error;
	FVA_EXIT_CODE res = fvaLoadDictionary( m_dictPath, m_dictionaries, error );
	RET_IF_RES_IS_ERROR

	updateDictionaryGUI();
}

FVADescriptionEditor::FVADescriptionEditor(
	bool							forFolder,
	const QStringList&				titles,
	QMap< QString, QStringList >&	decsItems,
	const QVariantMap&				dictionaries,
	int								indexOfFile,
	const QStringList&				files,
	const QString&					dictPath,
	const QString&					folderPath,
	QWidget *						parent)
	:
	QMainWindow(parent),
	m_dictionaries(dictionaries),
	m_dictPath(dictPath),
	m_forFolder(forFolder),
	m_titles(titles),
	m_decsItems(decsItems),
	m_indexOfFile(indexOfFile),
	m_files(files),
	m_folderPath(folderPath)
{
	init();

	updateGuiForItem(forFolder ? folderPath : m_files[m_indexOfFile]);

	QIcon	icon = QIcon(QCoreApplication::applicationDirPath() + "/Icons/main.png");
	setWindowIcon(icon);
}*/