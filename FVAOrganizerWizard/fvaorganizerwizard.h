#ifndef FVAORGANIZERWIZARD_H
#define FVAORGANIZERWIZARD_H

#include <QtWidgets/QWizard>
#include "ui_fvaorganizerwizard.h"

#include "fvacommonlib.h"

class FVAOrganizerWizard : public QWizard
{
	Q_OBJECT

	public:
		FVAOrganizerWizard(QWidget *parent = 0);
		~FVAOrganizerWizard();

		void inputFolder(const QString& inputFolder) { m_inputDir = inputFolder;}
		QString inputFolder(){return m_inputDir;}

		void matchedDeviceName(const QString& deviceName) { m_deviceName = deviceName;}
		QString matchedDeviceName(){return m_deviceName;}

		void fullDeviceMap(const DEVICE_MAP& fullDeviceMap_) {m_fullDeviceMap = fullDeviceMap_;}
		DEVICE_MAP fullDeviceMap() {return m_fullDeviceMap;}
		
		void matchedDeviceMap(const DEVICE_MAP& matchedDeviceMap_) {m_matchedDeviceMap = matchedDeviceMap_;}
		DEVICE_MAP matchedDeviceMap() {return m_matchedDeviceMap;}

		void inputDirType(FVA_EXIT_CODE	inputDirType_) { m_inputDirType = inputDirType_; }
		FVA_EXIT_CODE inputDirType() { return m_inputDirType; }

		void matchedDeviceID(int matchedDeviceID_) { m_matchedDeviceID = matchedDeviceID_; }
		int matchedDeviceID() { return m_matchedDeviceID; }

	private:

		Ui::FVAOrganizerWizardClass ui;

		QString			m_inputDir;
		QString			m_deviceName;
		DEVICE_MAP		m_fullDeviceMap;
		DEVICE_MAP		m_matchedDeviceMap;
		FVA_EXIT_CODE	m_inputDirType; // dir type is returned as exit code 
		int				m_matchedDeviceID;
};

#endif // FVAORGANIZERWIZARD_H
