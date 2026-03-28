/*!
 * \file fvadevice.h
 * \copyright Copyright 2021 FVA Software. All rights reserved. This file is released under the XXX License.
 * \author Dima Nikulin.
 * \version 0.29
 * \date  2014-2021
 */
#ifndef FVADEVICE_H
#define FVADEVICE_H

#include <map>
#include <string>

#include "fvaconstants.h"

/*!
 * \brief it describes whole information about device
 */
class fvaDevice {
public:
    /*
     * device identifier from device dictionary
     */
    int deviceId;

    /*
     * owner name from people dictionary
     */
    std::string ownerName;

    /*
     * owner identifier from people dictionary
     */
    int ownerId;

    /*
     * device name to register in dictionary(usually <brand name> + <device model>)
     */
    std::string linkedName;

    /*
     * device name to show in GUI
     */
    std::string guiName;

    /*
     * device type
     */
    FVA_DEVICE_TYPE type;

    /*
     * constructor
     */
    fvaDevice() : deviceId(0), ownerId(0), type(FVA_DEVICE_TYPE::DEVICE_TYPE_PHOTO_UKNOWN) {}
};

typedef std::map<int, fvaDevice> DEVICE_MAP;

#endif  // FVADEVICE_H