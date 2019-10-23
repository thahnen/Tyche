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
	CL_CAMERA_INIT,			// CameraListener: can not initialize camera
	CL_CAMERA_INIT_RAV,		// CameraListener: read access violation in camera initialization
	CL_LENS_PARAM,			// CameraListener: can not get camera lens parameters
	CL_EXPOSURE_MODE,		// CameraListener: can not set camera exposure mode

	UI_FILESAVEDIALOG,		// UI: could not get path for saving the file (using Windows Common Item Dialog)
	UI_HOME_DIRECTORY,		// UI: could not get users home directory
} TSTATUS;


/**
 *	Handles the TSTATUS error codes
 *
 *	@param						the given error code!
 *	@Return						if program can go on or not!
 */
bool handleTSTATUS(TSTATUS error) {
	// Can program recover from the error or not!
	bool recover = false;

	// Information for the message box
	LPCTSTR msgbox_text;
	UINT msgbox_type;

	switch (error) {
		case SUCCESS:
			// Success was given (not a particular error but ok)
			return true;
			
		case SH_GETWORKAREA:
			// Can not request resolution!
			msgbox_text = L"Can not request monitor resolution!\nMaybe Windows Version not supported?";
			msgbox_type = MB_OK | MB_ICONWARNING;
			break;
		case SH_SMALL_WIDTH:
		case SH_SMALL_HEIGHT:
		case SH_SMALL_HEIGHT_TB:
			// Monitor resolution
			// TODO: add ui resolution to information text!
			msgbox_text = L"Monitor resolution to low for the program to run!";
			msgbox_type = MB_OK | MB_ICONSTOP;
			break;

		case CL_LIST_EMPTY:
		case CL_LIST_OOB:
			// Maybe camera not connected (ask to retry)
			msgbox_text = L"Camera not found, maybe not connected?\nPlug in and retry or cancel!";
			msgbox_type = MB_RETRYCANCEL | MB_ICONQUESTION;
			recover = true;
			break;
		case CL_LIST_NULLPTR:
		case CL_CAMERA_INIT:
		case CL_CAMERA_INIT_RAV:
			// Camera is not accessable, possible error inside royale library
			msgbox_text = L"Camera not accessable nor initializable.\nProblem with the Camera!";
			msgbox_type = MB_OK | MB_ICONSTOP;
			break;
		case CL_LENS_PARAM:
		case CL_EXPOSURE_MODE:
			// Can not get/ change camera properties
			msgbox_text = L"Camera parameters can not be accessed.\nProblem with the Camera!";
			msgbox_type = MB_OK | MB_ICONWARNING;
			break;

		case UI_FILESAVEDIALOG:
			// No possibility to save to user set path! Using users home directory!
			msgbox_text = L"Can not save to specific path.\nUsing users home directory!";
			msgbox_type = MB_OK | MB_ICONINFORMATION;
			recover = true;
			break;
		case UI_HOME_DIRECTORY:
			// No possibility to save to users home directory! Strictly using default path (C:\)
			msgbox_text = L"Can not save to users home directory.\nUsing default path C:\\";
			msgbox_type = MB_OK | MB_ICONINFORMATION;
			recover = true;
			break;
		default:
			// New and/or not implemented error occured!
			msgbox_text = L"Unknown error occured!\nStopping the program!";
			msgbox_type = MB_OK | MB_ICONSTOP;
			break;

		// Show error dialog!
		int msgboxid = MessageBox(
			NULL,
			msgbox_text,
			L"An error occured!",
			msgbox_type
		);

		// When error is recoverable but user wants to exit anyways
		if (recover && msgboxid == IDCANCEL)	recover = false;

		return recover;
	}
}
