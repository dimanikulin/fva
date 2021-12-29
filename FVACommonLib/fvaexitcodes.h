/*!
* \file FVAexitcodes.h
* \copyright Copyright 2021 FVA Software. All rights reserved. This file is released under the XXX License.
* \author Dima Nikulin.
* \version 0.29
* \date  2014-2021
*/

#ifndef FVAERRORS_H
#define FVAERRORS_H

/*!
 * code errors enumeration
 */
enum FVA_EXIT_CODE
{
	FVA_NO_ERROR				= 0,
	
	FVA_ERROR_NOT_ENOUGH_ARG		= 1001,
	FVA_ERROR_INPUT_DIR_NOT_EXIST_ARG	= 1002,
	FVA_ERROR_CANT_OPEN_DIR_DESC		= 1003,
	FVA_ERROR_CANT_RENAME_DIR_DESC		= 1004,
	FVA_ERROR_CANT_OPEN_NEW_DIR_DESC	= 1005,
	FVA_ERROR_NOT_SUPPORTED_RO_MODE		= 1006,
	FVA_ERROR_CANT_RENAME_FILE		= 1007,
	FVA_ERROR_SEQUENCE			= 1008,
	FVA_ERROR_CANT_OPEN_INPUT_FILE		= 1009,
	FVA_ERROR_UKNOWN_CMD			= 1010,
	FVA_ERROR_CANT_FIND_MANDATORY_FIELDS	= 1011,
	FVA_ERROR_INCORRECT_FORMAT		= 1012,
	FVA_ERROR_CANT_REMOVE_FILE_OR_DIR	= 1013,
	FVA_ERROR_CANT_OPEN_FILE_DESC		= 1014,
	FVA_ERROR_CANT_FIND_DIR_DESC		= 1015,
	FVA_ERROR_EMPTY_DEV_ID			= 1016,
	FVA_ERROR_CANT_FIND_DICTIONARIES	= 1017,
	FVA_ERROR_CANT_OPEN_DICTIONARIES	= 1018,
	FVA_ERROR_CANT_LOAD_DICTIONARIES	= 1019,
	FVA_ERROR_WRONG_PARAMETERS		= 1020,
	FVA_ERROR_WRONG_FOLDER_NAME		= 1021,
	FVA_ERROR_WRONG_FILE_NAME		= 1022,
	FVA_ERROR_CANT_SAVE_DICTIONARIES	= 1023,
	FVA_ERROR_NOT_SUPPORTED_FILE		= 1024,
	FVA_ERROR_VIDEO_FIRST			= 1025,
	FVA_ERROR_PANORAM_FILE			= 1026,
	FVA_ERROR_EMPTY_DEVICE_NAME		= 1027,
	FVA_ERROR_NON_UNIQUE_DEVICE_NAME	= 1028,
	FVA_ERROR_UKNOWN_DEVICE			= 1029,
	FVA_ERROR_TOO_LITTLE_FILES		= 1030,
	FVA_ERROR_CANT_MOVE_DIR			= 1031,
	FVA_ERROR_CANT_CREATE_DIR		= 1032,
	FVA_ERROR_CANT_CREATE_FVA_SQL		= 1033,
	FVA_ERROR_CANT_RENAME_FILE_DESC		= 1034,
	FVA_ERROR_CANT_FIND_FVA_DB		= 1035,
	FVA_ERROR_CANT_OPEN_FVA_DB		= 1036,
	FVA_ERROR_CANT_LOAD_FVA_DB		= 1037,
	FVA_ERROR_NOT_IMPLEMENTED		= 1038,
	FVA_ERROR_CANT_OPEN_ID_FILE		= 1039,
	FVA_ERROR_NO_DEV_ID			= 1040,
	FVA_ERROR_NON_UNIQUE_FVA_INFO		= 1041,
	FVA_ERROR_EMPTY_VIDEO_TIME		= 1042,
	FVA_ERROR_DEST_FILE_ALREADY_EXISTS	= 1043,
	FVA_ERROR_DEST_DIR_ALREADY_EXISTS	= 1044,
	FVA_ERROR_NON_UNIQUE_FILE_NAME		= 1045,
	FVA_ERROR_CANT_SET_PARAM		= 1046,
	FVA_ERROR_CANT_GET_PARAM		= 1047,
	FVA_ERROR_INCORRECT_FILE_FORMAT		= 1048,
	FVA_ERROR_NO_EXIF_DATE_TIME		= 1049,
	FVA_ERROR_NO_EXIF_LOCATION		= 1050,
	FVA_ERROR_CANT_FIX_EMPTY_EXIF_DATE_TIME	= 1051,
	FVA_ERROR_CANT_START_PYTHON_PROC	= 1052,
	FVA_ERROR_CANT_EXIT_PYTHON_PROC		= 1053,
};

#define RET_RES_IF_RES_IS_ERROR\
	if ( FVA_NO_ERROR != res )\
		return res;

#define RET_IF_RES_IS_ERROR\
	if ( FVA_NO_ERROR != res )\
		return;

#endif // FVAERRORS_H
