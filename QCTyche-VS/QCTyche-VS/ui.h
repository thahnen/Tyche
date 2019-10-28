#pragma once

#include <string>
#include <ctime>
#include <ShlObj.h>
#include <KnownFolders.h>
#include <opencv2/opencv.hpp>

#include "Status.h"


#define WINDOW_NAME			"QCTyche"
#define WINDOW_WIDTH		640
#define WINDOW_HEIGHT		480

#define BTN_SAVE_TEXT		"Save image to file"
#define BTN_SAVE_WIDTH		140
#define BTN_SAVE_HEIGHT		30
#define BTN_SAVE_X			250
#define BTN_SAVE_Y			380

#define BTN_PRE_TEXT		"Change Preview"
#define BTN_PRE_WIDTH		140
#define BTN_PRE_HEIGHT		30
#define BTN_PRE_X			250
#define BTN_PRE_Y			420

#define BTN_START_TEXT		"Start"
#define BTN_START_WIDTH		70
#define BTN_START_HEIGHT	30
#define BTN_START_X			150
#define BTN_START_Y			380

#define BTN_STOP_TEXT		"Stop"
#define BTN_STOP_WIDTH		70
#define BTN_STOP_HEIGHT		30
#define BTN_STOP_X			150
#define BTN_STOP_Y			420

#define BTN_QUIT_TEXT		"Quit"
#define BTN_QUIT_WIDTH		70
#define BTN_QUIT_HEIGHT		30
#define BTN_QUIT_X			420
#define BTN_QUIT_Y			400

#define IMG_WIDTH			448
#define IMG_HEIGHT			342
#define IMG_X				96
#define IMG_Y				20


#define DEFAULT_PATH		"C:\\"


/**
 *	Requests a path to save the image at from the user
 *
 *	@param file_path		where to store the path
 *	@return					SUCCESS if the resolution matches the requirements otherwise an error
 */
TSTATUS getFileSavePath(std::string& file_path) {
	IFileSaveDialog* fsd;

	// Create FileOpenDialog
	HRESULT res = CoCreateInstance(
		CLSID_FileSaveDialog, NULL, CLSCTX_ALL, IID_IFileSaveDialog,
		reinterpret_cast<void**>(&fsd)
	);

	if (SUCCEEDED(res)) {
		// Set title
		res = fsd->SetTitle(L"Save image as ('.png' added automatically)");

		// Set png as file extension!
		res = fsd->SetDefaultExtension(L"png");

		// Show dialog box
		res = fsd->Show(NULL);

		if (SUCCEEDED(res)) {
			// Get the resulting item
			IShellItem* item;
			res = fsd->GetResult(&item);

			if (SUCCEEDED(res)) {
				// Request item path
				LPWSTR path;
				res = item->GetDisplayName(SIGDN_FILESYSPATH, &path);

				if (SUCCEEDED(res)) {
					// Convert path to std::string for easier access
					std::wstring wpath = path;
					file_path = std::string(wpath.begin(), wpath.end());

					// TODO: auskommentieren!
					std::cout << "Dateipfad zum speichern: " << file_path << std::endl;

					// Release item and return successfully
					item->Release();
					return SUCCESS;
				}
			}
		}
	}

	// Return non-specific error if requesing save path fails!
	return UI_FILESAVEDIALOG;
}


/**
 *	Requests the users home directory as alternative to personally set path!
 *
 *	@param file_path		where to store the path
 *	@return					SUCCESS if the resolution matches the requirements otherwise an error
 */
TSTATUS getUserDirectory(std::string& file_path) {
	return SUCCESS;
}



/**
 *	Handler for mouse actions (cvui::CLICK)
 *
 *	@param x				the x coordinate where the mouse clicked
 *	@param y				the y coordunate where the mouse clicked
 *	@param displayDepth		the variable which indicates if the depth or the grayscale image is displayed
 *	@param captureNew		the variable which indicates if a new image should be requested every loop or not
 *	@param image			the varuable which holds the current image and is used for saving it
 *	@return					SUCCESS if the resolution matches the requirements otherwise an error
 *
 *	TODO: create smaller handlers for buttons etc!
 */
TSTATUS handleMouseInput(int x, int y, bool* displayDepth, bool* captureNew, cv::Mat* image) {
	std::string path;

	if (x >= BTN_QUIT_X && x <= BTN_QUIT_X + BTN_QUIT_WIDTH && y >= BTN_QUIT_Y && y <= BTN_QUIT_Y + BTN_QUIT_HEIGHT) {
		// Quit gedr�ckt! (noch eigenen "Fehler ausdenken)
		return CD_REGISTER_LIST;
	} else if (x >= BTN_PRE_X && x <= BTN_PRE_X + BTN_PRE_WIDTH && y >= BTN_PRE_Y && y <= BTN_PRE_Y + BTN_PRE_HEIGHT) {
		// Change the preview to the opposite image
		*displayDepth = !(*displayDepth);
	} else if (*captureNew && x >= BTN_STOP_X && BTN_STOP_X + BTN_STOP_WIDTH && y >= BTN_STOP_Y && y <= BTN_STOP_Y + BTN_STOP_HEIGHT) {
		// Only if camera is capturing and "Stop" is pressed stop capturing
		*captureNew = false;
	} else if (!(*captureNew) && x >= BTN_START_X && x <= BTN_START_X + BTN_START_WIDTH && y >= BTN_START_Y && y <= BTN_START_Y + BTN_START_HEIGHT) {
		// Only if camera is not capturing and "Start" is pressed start capturing again
		*captureNew = true;
	} else if (x >= BTN_SAVE_X && x <= BTN_SAVE_X + BTN_SAVE_WIDTH && y >= BTN_SAVE_Y && y <= BTN_SAVE_Y + BTN_SAVE_HEIGHT) {
		// Save the current image
		std::string path;

		// 1) Get a path to save to
		if (getFileSavePath(path) != SUCCESS) {
			// Can not request save path, get user home path instead
			PWSTR ppath;
			HRESULT hr = SHGetKnownFolderPath(FOLDERID_Desktop, 0, NULL, &ppath);
			if (SUCCEEDED(hr)) {
				std::wstring wpath = ppath;
				path = std::string(wpath.begin(), wpath.end());
			} else {
				// Can not request user desktop path, get C:\ instead
				path = "C:\\";
			}

			// as no file name was given, a new one is created from timestamp
			std::time_t timestamp = std::time(nullptr);
			path += std::asctime(std::localtime(&timestamp));
			path += ".png";

			// replace every space with underscore
			std::transform(path.begin(), path.end(), path.begin(), [](char ch) {
				return ch == ' ' ? '_' : ch;
			});
		}

		// 2) Save image to file using requested path
		if (cv::imwrite(path, *image)) {
			// Can not save image to file!
		}
	}

	return getFileSavePath(path);
}
