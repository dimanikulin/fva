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
};
#endif

