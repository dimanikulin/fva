#ifndef FVAORGANIZERWIZARD_H
#define FVAORGANIZERWIZARD_H

#include <QtWidgets/QWizard>
#include "ui_fvaorganizerwizard.h"

#include "fvacommonlib.h"
#include "fvacommondata.h"

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

	private:

		Ui::FVAOrganizerWizardClass ui;

		QString			m_inputDir;
		QString			m_deviceName;
		DEVICE_MAP		m_fullDeviceMap;
		DEVICE_MAP		m_matchedDeviceMap;
};

#endif // FVAORGANIZERWIZARD_H
