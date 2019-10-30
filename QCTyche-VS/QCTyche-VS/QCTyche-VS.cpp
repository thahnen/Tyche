// QCTyche-VS.cpp : Diese Datei enthält die Funktion "main". Hier beginnt und endet die Ausführung des Programms.
//

#include <iostream>
#include <chrono>
#include <Windows.h>
#include <opencv2/opencv.hpp>
#include <royale.hpp>

#define CVUI_DISABLE_COMPILATION_NOTICES
#define CVUI_IMPLEMENTATION
#include "cvui.h"

#include "SystemHandler.h"
#include "CameraListener.h"
#include "ui.h"

using namespace std;


/// Starts the user interface once and for all (update done in loop)
TSTATUS startUI(cv::Mat& window);



/**
 *	Main program running
 *	=> no possible command line arguments yet, may change in the future!
 *
 *	BUG: When OpenCV window was closed not using the "Quit" button the UI could not respond to any mouse action!
 *	TODO: Maybe handle mouse actions not using CVUI but using own mechanism?
 */
int main(int argc, char** argv) {
	/// Status variable used throughout the program to handle return states
	TSTATUS stat;


	///	Check the system requirements (monitor resolution, ...)
	///
	cout << "Test system on functionality!" << endl;
	if ((stat = checkResolutionRequirements(WINDOW_WIDTH, WINDOW_HEIGHT)) != SUCCESS) {
		// TODO: maybe create resolvable error?
		handleTSTATUS(stat);
		return 1;
	}


	///	The window variable which holds the primitive GUI
	cv::Mat window;


	///	Try to start the gui before minimizing the console window which is only used for logging!
	///
	if ((stat = startUI(window)) != SUCCESS) {
		handleTSTATUS(stat);
		return 2;
	}
	cvui::update();
	cvui::imshow(WINDOW_NAME, window);

	ShowWindow(GetConsoleWindow(), SW_MINIMIZE);


	/// Camera listener used for setting up the camera and handling the input every frame
	/// The camera device which is representing the camera
	CameraListener listener;
	unique_ptr<royale::ICameraDevice> cameraDevice;


	///	Try to request a camera until one is plugged in or the user cancels the application
	///
	while ((stat = listener.requestCamera(cameraDevice)) != SUCCESS) {
		// Wait until camera is plugged in or cancel is pressed (error can be recovered!)
		if (!handleTSTATUS(stat)) return 3;
	}


	///	Try to configure the camera by setting lens parameters and exposure mode
	///
	if ((stat = listener.configureCamera(cameraDevice)) != SUCCESS) {
		// Camera settings can not be configured (error can not be recovered!)
		handleTSTATUS(stat);
		return 4;
	}


	///	Try to register our camera listener to the specific device!
	///
	if (cameraDevice->registerDataListener(&listener) != royale::CameraStatus::SUCCESS) {
		// Camera can not register listener to get images (error can not be recovered!)
		handleTSTATUS(CD_REGISTER_LIST);
		return 5;
	}


	///	Try to start capturing images using the camera
	///
	if (cameraDevice->startCapture() != royale::CameraStatus::SUCCESS) {
		// Camera can not start recording video (error can not be recovered!)
		handleTSTATUS(CD_START_CAPTURE);
		return 6;
	}


	/// Sleeping for camera to capture first images before requesting one!
	Sleep(1000);


	/// Camera image for access over more than one loop (non-resized CV_8UC3 and CV_32F matrix)
	/// Variable indicating which image is displayed (grayscale or depth)
	/// Variable indicating if image "capturing" is stopped
	cv::Mat current_image;
	cv::Mat current_float_mat;
	bool displayDepth = true;
	bool captureNew = true;


	/// To measure the time difference in which the preview image has to be refreshed
	/// Variable holds the last time the image was refreshed
	auto last = std::chrono::steady_clock::now();
	uint16_t fps;
	if (cameraDevice->getFrameRate(fps) != royale::CameraStatus::SUCCESS) {
		// Can not get camera frame rate, just set to 30 fps
		fps = 30;
	}


	///	The main loop of the program (which runs until the user pressed the "Quit" button)
	///
	for (;;) {
		cv::Point cursor = cvui::mouse(WINDOW_NAME);

		// TODO: ui function for handling button presses etc!
		if (cvui::mouse(cvui::DOWN)) {
			cout << "Mouse clicked -> x:" << cursor.x << " y:" << cursor.y << endl;

			if ((stat = handleMouseInput(
					cursor.x, cursor.y, displayDepth, captureNew, current_image, current_float_mat
				)) == UI_QUIT) {
				break;
			}

			if (stat != SUCCESS) {
				handleTSTATUS(stat);
			}
		}

		// Check if the time of the last image update is bigger than the cameras frame rate
		if (std::chrono::duration_cast<std::chrono::milliseconds>(
				std::chrono::steady_clock::now() - last
			).count() >= (long long)std::floor(1000 / fps)) {
			// Request new image (if "capturing" not stopped)
			if (captureNew) {
				std::string pre_text("Preview: ");

				// Which image should be requested?
				if (displayDepth) {
					current_image = listener.getNewestDepthImage(CV_8UC1);
					current_float_mat = listener.getNewestDepthImage();
					pre_text += "Depth";
				} else {
					current_image = listener.getNewestGrayscaleImage(CV_8UC1);
					current_float_mat = listener.getNewestGrayscaleImage();
					pre_text += "Grayscale";
				}

				cv::Mat resized = current_image.clone();
				cv::resize(resized, resized, cv::Size(resized.cols * 2, resized.rows * 2), 0, 0, cv::INTER_LINEAR);
				cv::cvtColor(resized, resized, cv::COLOR_GRAY2BGR);
				
				// Apply color map if it is depth image
				if (displayDepth) {
					cv::applyColorMap(resized, resized, cv::COLORMAP_COOL);
				}

				// Overshadow old preview text
				cvui::rect(
					window, TXT_PRE_X - 10, TXT_PRE_Y - 5, 200, 40, WINDOW_COLOR, WINDOW_COLOR
				);

				// Update preview text
				cvui::text(
					window, TXT_PRE_X, TXT_PRE_Y, pre_text, TXT_PRE_SCALE, TXT_PRE_COLOR
				);

				// Update preview window
				cvui::image(
					window, IMG_X, IMG_Y, resized
				);
			}

			// Update the timer
			last = std::chrono::steady_clock::now();
		}

		// Update the user interface
		cvui::update();
		cvui::imshow(WINDOW_NAME, window);

		// For now break from endless loop only using escape key!
		// TODO: remove this later, by now use it when "Quit" button may not work!
		if (cv::waitKey(1) == 27) break;
	}


	///	Try to stop capturing images using the camera
	///
	if (cameraDevice->stopCapture() != royale::CameraStatus::SUCCESS) {
		// Camera can not stop recording images (not harmful as this happens at the end!)
		handleTSTATUS(CD_STOP_CAPTURE);
	}


	cout << "Stops running ..." << endl;
	return 0;
}



/**
 *	Starts the user interface once and for all (update done in loop)
 *
 *	@param window			the user interface window
 *	@return					SUCCESS if the user interface could be set up otherwise an error
 */
TSTATUS startUI(cv::Mat& window) {
	try {
		// Create window and set background
		window = cv::Mat(cv::Size(WINDOW_WIDTH, WINDOW_HEIGHT), CV_8UC3);
		window = cv::Scalar(157, 161, 170);

		// Initialize cvui
		cvui::init(WINDOW_NAME);

		// Set UI elements:
		// 1) Save button
		cvui::button(
			window, BTN_SAVE_X, BTN_SAVE_Y, BTN_SAVE_WIDTH, BTN_SAVE_HEIGHT, BTN_SAVE_TEXT
		);

		// 2) Change preview button (from depth to grayscale and vise versa)
		cvui::button(
			window, BTN_PRE_X, BTN_PRE_Y, BTN_PRE_WIDTH, BTN_PRE_HEIGHT, BTN_PRE_TEXT
		);

		// 3) Start button (to resume camera live feed)
		cvui::button(
			window, BTN_START_X, BTN_START_Y, BTN_START_WIDTH, BTN_START_HEIGHT, BTN_START_TEXT
		);

		// 4) Stop button (to stop camera live feed at current image)
		cvui::button(
			window, BTN_STOP_X, BTN_STOP_Y, BTN_STOP_WIDTH, BTN_STOP_HEIGHT, BTN_STOP_TEXT
		);

		// 5) Quit button (to quit application as closing the OpenCV windows does not work
		cvui::button(
			window, BTN_QUIT_X, BTN_QUIT_Y, BTN_QUIT_WIDTH, BTN_QUIT_HEIGHT, BTN_QUIT_TEXT
		);

		// 6) Text for preview (showing which image is displayed)
		cvui::text(
			window, TXT_PRE_X, TXT_PRE_Y, TXT_PRE_TEXT, TXT_PRE_SCALE, TXT_PRE_COLOR
		);

		// 7) Preview of camera (test image before starting to capture)
		cv::Mat testbild = cv::imread("testbild.png");
		if (!testbild.data) {
			// image could not be loaded
			testbild = cv::Mat(cv::Size(IMG_WIDTH, IMG_HEIGHT), CV_8UC3);
			testbild = cv::Scalar(0, 0, 255);
		}

		cvui::image(
			window, IMG_X, IMG_Y, testbild
		);
	} catch (const std::exception& e) {
		// TODO: log the error?
		return UI_SETUP;
	}

	return SUCCESS;
}
