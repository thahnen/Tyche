#pragma once

#include <string>
#include <ctime>
#include <ShlObj.h>
#include <KnownFolders.h>
#include <opencv2/opencv.hpp>

#include "Status.h"
#include "FileHandler.h"


#define WINDOW_NAME			"QCTyche"
#define WINDOW_WIDTH		640
#define WINDOW_HEIGHT		480
#define WINDOW_COLOR		0xAAA19D

#define BTN_SAVE_TEXT		"Save image to file"
#define BTN_SAVE_WIDTH		140
#define BTN_SAVE_HEIGHT		30
#define BTN_SAVE_X			250
#define BTN_SAVE_Y			390

#define BTN_PRE_TEXT		"Change Preview"
#define BTN_PRE_WIDTH		140
#define BTN_PRE_HEIGHT		30
#define BTN_PRE_X			250
#define BTN_PRE_Y			430

#define BTN_START_TEXT		"Start"
#define BTN_START_WIDTH		70
#define BTN_START_HEIGHT	30
#define BTN_START_X			150
#define BTN_START_Y			390

#define BTN_STOP_TEXT		"Stop"
#define BTN_STOP_WIDTH		70
#define BTN_STOP_HEIGHT		30
#define BTN_STOP_X			150
#define BTN_STOP_Y			430

#define BTN_QUIT_TEXT		"Quit"
#define BTN_QUIT_WIDTH		70
#define BTN_QUIT_HEIGHT		30
#define BTN_QUIT_X			420
#define BTN_QUIT_Y			410

#define TXT_PRE_TEXT		"Preview: NULL"
#define TXT_PRE_SCALE		0.6
#define TXT_PRE_COLOR		0x323232
#define TXT_PRE_X			250
#define TXT_PRE_Y			10

#define IMG_WIDTH			448
#define IMG_HEIGHT			342
#define IMG_X				96
#define IMG_Y				35


#define DEFAULT_PATH		"C:\\"


/**
 *	Requests a path to save the image at from the user
 *
 *	@param file_path		where to store the path
 *	@return					SUCCESS if the resolution matches the requirements otherwise an error
 *
 *	BUG: Wenn Speicherort vom Benutzer gew�hlt aber kein Bild unter dem Pfad gespeichert wurde
 *	BUG: taucht die "Datei" trotzdem im Explorer unter "Zuletzt verwendete Dateien" auf!
 *
 *	TODO: Bug bei MS melden!
 *	TODO: Handle clicking "Cancel" in window!
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

		// Return immediately if "Cancel" was pressed
		if (res == HRESULT_FROM_WIN32(ERROR_CANCELLED))	return UI_SAVE_CANCELLED;

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
 *	Requests the users desktop directory as alternative to personally set path!
 *
 *	@param file_path		where to store the path
 *	@return					SUCCESS if the resolution matches the requirements otherwise an error
 */
TSTATUS getUserDesktopDirectory(std::string& file_path) {
	PWSTR ppath;
	HRESULT hr = SHGetKnownFolderPath(FOLDERID_Desktop, 0, NULL, &ppath);

	if (SUCCEEDED(hr)) {
		std::wstring wpath = ppath;
		file_path = std::string(wpath.begin(), wpath.end());

		return SUCCESS;
	}

	return UI_HOME_DIRECTORY;
}


#pragma warning(push)
#pragma warning(disable: 4996)
/**
 *	Handler for mouse actions (cvui::CLICK)
 *
 *	@param x				the x coordinate where the mouse clicked
 *	@param y				the y coordunate where the mouse clicked
 *	@param displayDepth		the variable which indicates if the depth or the grayscale image is displayed
 *	@param captureNew		the variable which indicates if a new image should be requested every loop or not
 *	@param image			the varuable which holds the current image and is used for saving it
 *	@param mat_float		the variable which holds the current float matrix and is used for saving it
 *	@return					SUCCESS if the resolution matches the requirements otherwise an error
 *
 *	TODO: Maybe inform user where image was saved!
 */
TSTATUS handleMouseInput(int x, int y, bool& displayDepth, bool& captureNew, cv::Mat& image, cv::Mat& mat_float) {
	TSTATUS stat = SUCCESS;

	if (x >= BTN_QUIT_X && x <= BTN_QUIT_X + BTN_QUIT_WIDTH && y >= BTN_QUIT_Y && y <= BTN_QUIT_Y + BTN_QUIT_HEIGHT) {
		// Quit button pressed, ending the application!
		stat = UI_QUIT;
	} else if (x >= BTN_PRE_X && x <= BTN_PRE_X + BTN_PRE_WIDTH && y >= BTN_PRE_Y && y <= BTN_PRE_Y + BTN_PRE_HEIGHT) {
		// Change the preview to the opposite image
		displayDepth = !(displayDepth);
	} else if (captureNew && x >= BTN_STOP_X && BTN_STOP_X + BTN_STOP_WIDTH && y >= BTN_STOP_Y && y <= BTN_STOP_Y + BTN_STOP_HEIGHT) {
		// Only if camera is capturing and "Stop" is pressed stop capturing
		captureNew = false;
	} else if (!(captureNew) && x >= BTN_START_X && x <= BTN_START_X + BTN_START_WIDTH && y >= BTN_START_Y && y <= BTN_START_Y + BTN_START_HEIGHT) {
		// Only if camera is not capturing and "Start" is pressed start capturing again
		captureNew = true;
	} else if (x >= BTN_SAVE_X && x <= BTN_SAVE_X + BTN_SAVE_WIDTH && y >= BTN_SAVE_Y && y <= BTN_SAVE_Y + BTN_SAVE_HEIGHT) {
		// Save the current image
		std::string path;

		// 1) Get a path to save to
		stat = getFileSavePath(path);
		if (stat == UI_SAVE_CANCELLED)	return stat;

		if (stat != SUCCESS) {
			// Can not request save path, get user home path instead

			std::cout << "getFileSavePath failed!" << std::endl;

			if ((stat = getUserDesktopDirectory(path)) != SUCCESS) {
				// Can not request user desktop directory path, using C:\ instead
				path = DEFAULT_PATH;
			}

			path += "\\";

			// as no file name was given, a new one is created from timestamp
			std::string filename = "camera.";
			std::ostringstream oss;
			std::time_t t = std::time(nullptr);

			oss << std::put_time(std::localtime(&t), "%c");
			filename += oss.str();

			// replace every space + forward slash + colon with underscore
			std::transform(filename.begin(), filename.end(), filename.begin(), [](char ch) {
				if (ch == ' ' || ch == '/' || ch == ':') return '_';
				return ch;
			});

			path += filename;
		}

		// 2) Save image to file using requested path
		stat = saveMatrix(path, image, mat_float);
	}

	return stat;
}
#pragma warning(pop)
