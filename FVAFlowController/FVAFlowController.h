#ifndef _FVA_FLOW_CONTROLLER_H_
#define _FVA_FLOW_CONTROLLER_H_

#include <QtCore/QString>

#include "fvaexitcodes.h"
#include "fvadevicecontext.h"

class QObject;

/*!
 * \brief it is a main class to keep a logic of the operation flow and interaction with a user 
 */
class FVAFlowController
{
public:
	FVAFlowController(){};
	virtual ~FVAFlowController(){};

	/*!
	 * \brief it performs the checks for input folder according to fva configuration
	 * \param dir - directory for the checking
	 * \param deviceContext - devices information got during checks, filled up by this function
	 * \param obj - to attach the child processes to this object
	 * \return it returns code of error (not FVA_NO_ERROR if error happened, FVA_NO_ERROR - if no error happened)
	*/
	FVA_EXIT_CODE PerformChecksForInputDir(const QString& dir, DeviceContext& deviceContext, QObject* obj);

private:

	/*!
	 * \brief it performs the device checks for input folder
	 * \param dir - directory for the checking
	 * \param deviceContext - devices information got during checks, filled up by this function
	 * \param rootSWdir - root directory where the fva sw is located
	 * \return it returns code of error (FVA_NO_ERROR - if no error happened)
	 */
	FVA_EXIT_CODE performDeviceChecks(const QString& dir, DeviceContext& deviceContext, const QString& rootSWdir);

	/*!
	 * \brief it performs the orientation checks for input folder and reorientate the photos for inproper orientation
	 * \param dir - directory for the checking
	 * \param obj - to attach the child processes to this object
	 * \return it returns void
	 */
	void performOrientationChecks(const QString& dir, QObject* obj);

	/*!
	 * \brief it performs the common checks for input folder
	 * \param dir - directory for the checking
	 * \return it returns code of error (FVA_NO_ERROR - if no error happened)	
	 */
	FVA_EXIT_CODE performCommonChecks(const QString& dir);

	/*!
	* \brief it performs the date-time checks for input folder
	* \param dir - directory for the checking
	* \return it returns code of error (FVA_NO_ERROR - if no error happened)
	*/
	FVA_EXIT_CODE performDTChecks(const QString& dir);

	/*!
	* \brief it performs the location checks for input folder
	* \param dir - directory for the checking
	* \return it returns code of error (FVA_NO_ERROR - if no error happened)
	*/
	FVA_EXIT_CODE performLocationChecks(const QString& dir);

};
#endif

