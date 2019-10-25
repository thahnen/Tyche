#pragma once

#include <Windows.h>


typedef enum {
	SUCCESS = 0,			// Success on requested function

	SH_GETWORKAREA,			// SystemHandler: error on getting screen size
	SH_SMALL_WIDTH,			// SystemHandler: screen is not wide enough
	SH_SMALL_HEIGHT,		// SystemHandler: screen is not tall enough
	SH_SMALL_HEIGHT_TB,		// SystemHandler: screen is not tall enough (in fullscreen)

	CL_LIST_EMPTY,			// CameraListener: list of cameras empty
	CL_LIST_OOB,			// CameraListener: list item out of bounds requested
	CL_LIST_NULLPTR,		// CameraListener: list item is nullprt
	CL_CAMERA_INIT,			// CameraListener: can not initialize camera
	CL_CAMERA_INIT_RAV,		// CameraListener: read access violation in camera initialization
	CL_LENS_PARAM,			// CameraListener: can not get camera lens parameters
	CL_EXPOSURE_MODE,		// CameraListener: can not set camera exposure mode

	CD_REGISTER_LIST,		// CameraDevice: error registering a listener
	CD_START_CAPTURE,		// CameraDevice: camera can not start capturing
	CD_STOP_CAPTURE,		// CameraDevice: camera can not stop capturing

	UI_FILESAVEDIALOG,		// UI: could not get path for saving the file (using Windows Common Item Dialog)
	UI_HOME_DIRECTORY,		// UI: could not get users home directory
} TSTATUS;


/**
 *	Handles the TSTATUS error codes
 *
 *	@param						the given error code!
 *	@Return						if program can go on or not!
 */
bool handleTSTATUS(TSTATUS error);
