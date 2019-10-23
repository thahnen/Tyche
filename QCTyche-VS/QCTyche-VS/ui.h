#pragma once

#include <string>
#include <ShObjIdl_core.h>

#include "Status.h"
#include <cstring>


#define WINDOW_NAME			"QCTyche"
#define WINDOW_WIDTH		640
#define WINDOW_HEIGHT		480

#define BTN_SAVE_TEXT		"Speichern"
#define BTN_SAVE_WIDTH		75
#define BTN_SAVE_HEIGHT		30
#define BTN_SAVE_X			10
#define BTN_SAVE_Y			10

#define BTN_PRE_TEXT		"Change Preview"
#define BTN_PRE_WIDTH		150
#define BTN_PRE_HEIGHT		30
#define BTN_PRE_X			100
#define BTN_PRE_Y			100


#define DEFAULT_PATH		"C:\"


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
TSTATUS getUserDirectory(std::string& file_path) {}



/**
 *	Handler for mouse actions (cvui::CLICK)
 *
 *	@param x				the x coordinate where the mouse clicked
 *	@param y				the y coordunate where the mouse clicked
 *	@return					SUCCESS if the resolution matches the requirements otherwise an error
 *
 *	TODO: create smaller handlers for buttons etc!
 */
TSTATUS handleMouseInput(int x, int y) {
	std::string path;

	return getFileSavePath(path);
}
