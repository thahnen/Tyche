// QCTyche-VS.cpp : Diese Datei enthält die Funktion "main". Hier beginnt und endet die Ausführung des Programms.
//

#include <iostream>
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
 *
 */
int main(int argc, char** argv) {
	cout << "Starts running ..." << endl;

	TSTATUS stat;


	cout << "Test system on functionality!" << endl;
	if ((stat = checkResolutionRequirements(WINDOW_WIDTH, WINDOW_HEIGHT)) != SUCCESS) {
		// TODO: maybe create resolvable error?
		handleTSTATUS(stat);
		return -1;
	}


	cv::Mat window;
	startUI(window);

	// Minimizes the console running this program!
	ShowWindow(GetConsoleWindow(), SW_MINIMIZE);

	cvui::update();

	cvui::imshow(WINDOW_NAME, window);


	CameraListener listener;

	unique_ptr<royale::ICameraDevice> cameraDevice;
	while ((stat = listener.requestCamera(cameraDevice)) != SUCCESS) {
		// Wait until camera is plugged in or cancel is pressed (error can be recovered!)
		if (!handleTSTATUS(stat)) return -2;
	}


	if ((stat = listener.configureCamera(cameraDevice)) != SUCCESS) {
		// Camera settings can not be configured (error can not be recovered!)
		handleTSTATUS(stat);
		return -3;
	}


	if (cameraDevice->registerDataListener(&listener) != royale::CameraStatus::SUCCESS) {
		// Camera can not register listener to get images (error can not be recovered!)
		handleTSTATUS(CD_REGISTER_LIST);
		return -4;
	}

	if (cameraDevice->startCapture() != royale::CameraStatus::SUCCESS) {
		// Camera can not start recording video (error can not be recovered!)
		handleTSTATUS(CD_START_CAPTURE);
		return -5;
	}

	/// Sleeping for camera to capture first images before requesting one!
	Sleep(1000);

	/// Camera image for access over more than one loop
	cv::Mat current_image;

	/// Which image is displayed (grayscale or depth)
	bool displayDepth = true;

	/// Image "capturing" is stopped
	bool captureNew = true;

	for (;;) {
		cv::Point cursor = cvui::mouse(WINDOW_NAME);

		// TODO: ui function for handling button presses etc!
		if (cvui::mouse(cvui::DOWN)) {
			cout << "Mouse clicked -> x:" << cursor.x << " y:" << cursor.y << endl;

			if (handleMouseInput(cursor.x, cursor.y, &displayDepth, &captureNew, &current_image) != SUCCESS) break;
		}

		// Update preview window
		cv::Mat current_depth = listener.getNewestDepthImage(CV_8UC1);
		cv::resize(current_depth, current_depth, cv::Size(current_depth.cols * 2, current_depth.rows * 2), 0, 0, cv::INTER_LINEAR);
		cv::cvtColor(current_depth, current_depth, cv::COLOR_GRAY2BGR);

		cvui::image(
			window, IMG_X, IMG_Y, current_depth
		);


		cvui::update();

		cvui::imshow(WINDOW_NAME, window);


		// For now break from endless loop only using escape key!
		if (cv::waitKey(1) == 27) break;
	}


	if (cameraDevice->stopCapture() != royale::CameraStatus::SUCCESS) {
		// Camera can not stop recording images (not harmful as this happens at the end!)
		handleTSTATUS(CD_STOP_CAPTURE);
	}


	cout << "Stops running ..." << endl;
}



/**
 *	Starts the user interface once and for all (update done in loop)
 *
 *	@param window			the user interface window
 */
TSTATUS startUI(cv::Mat& window) {
	// Create window and set background
	window = cv::Mat(cv::Size(WINDOW_WIDTH, WINDOW_HEIGHT), CV_8UC3);
	window = cv::Scalar(157, 161, 170);

	// Initialize cvui
	cvui::init(WINDOW_NAME);

	// Set UI elements
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

	// 3) Preview of camera
	cv::Mat testbild(cv::Size(IMG_WIDTH, IMG_HEIGHT), CV_8SC3);
	testbild = cv::Scalar(0, 0, 255);
	cvui::image(
		window, IMG_X, IMG_Y, testbild
	);

	return SUCCESS;
}
