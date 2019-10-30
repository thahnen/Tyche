#pragma once


typedef enum {
	SUCCESS = 0,			// Success on requested function

	SH_GETWORKAREA,			// SystemHandler: error on getting screen size
	SH_SMALL_WIDTH,			// SystemHandler: screen is not wide enough
	SH_SMALL_HEIGHT,		// SystemHandler: screen is not tall enough
	SH_SMALL_HEIGHT_TB,		// SystemHandler: screen is not tall enough (in fullscreen)

	CL_LIST_EMPTY,			// CameraListener: list of cameras empty
	CL_LIST_OOB,			// CameraListener: list item out of bounds requested
	CL_LIST_NULLPTR,		// CameraListener: list item is nullprt
	CL_MORE_CAMS,			// CameraListener: more than one camera in the list
	CL_CAMERA_INIT,			// CameraListener: can not initialize camera
	CL_CAMERA_INIT_RAV,		// CameraListener: read access violation in camera initialization
	CL_LENS_PARAM,			// CameraListener: can not get camera lens parameters
	CL_EXPOSURE_MODE,		// CameraListener: can not set camera exposure mode

	CD_REGISTER_LIST,		// CameraDevice: error registering a listener
	CD_START_CAPTURE,		// CameraDevice: camera can not start capturing
	CD_STOP_CAPTURE,		// CameraDevice: camera can not stop capturing

	UI_SETUP,				// UI: could not set up the UI (cvui.h may be a bit buggy sometimes)
	UI_QUIT,				// UI: user pressed "Quit" button to quit the application!
	UI_SAVE_CANCELLED,		// UI: user cancelled the saving process
	UI_FILESAVEDIALOG,		// UI: could not get path for saving the file (using Windows Common Item Dialog)
	UI_HOME_DIRECTORY,		// UI: could not get users home directory

	FH_SAVE_IMAGE,			// FileHandler: could not save image to path
	FH_SAVE_MATRIX,			// FileHandler: could not save float matrix to path
	FH_LOAD_NOT_FOUND,		// FileHandler: file to load was not found at given path
	FH_LOAD_MATRIX,			// FileHandler: could not load float matrix from file
} TSTATUS;


/**
 *	Handles the TSTATUS error codes
 *
 *	@param						the given error code!
 *	@Return						if program can go on or not!
 */
bool handleTSTATUS(TSTATUS error);
