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


/**
 *	Starts the user interface once and for all (update done in loop)
 *
 *	@param window			the user interface window
 */
TSTATUS startUI(cv::Mat& window);


int main() {
	cout << "Starts running ..." << endl;

	TSTATUS stat;


	cout << "Test system on functionality!" << endl;
	if ((stat = checkResolutionRequirements(WINDOW_WIDTH, WINDOW_HEIGHT)) != SUCCESS) {
		// TODO: maybe create resolvable error?
		handleTSTATUS(stat);
		return -1;
	}


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

	/// Test sleeping!
	Sleep(1000);

	cv::Mat window;
	startUI(window);

	// Minimizes the console running this program!
	ShowWindow(GetConsoleWindow(), SW_MINIMIZE);


	for (;;) {
		//cv::Mat current_grayscale = listener.getNewestGrayscaleImage(CV_8UC1);
		//cv::Mat current_depth = listener.getNewestDepthImage(CV_8UC1);

		//cv::imshow("Grayscale", current_grayscale);
		//cv::imshow("Depth", current_depth);

		cv::Point cursor = cvui::mouse(WINDOW_NAME);

		// TODO: ui function for handling button presses etc!
		if (cvui::mouse(cvui::DOWN)) {
			cout << "Mouse clicked -> x:" << cursor.x << " y:" << cursor.y << endl;

			handleMouseInput(cursor.x, cursor.y);
		}


		cvui::update();

		cvui::imshow(WINDOW_NAME, window);


		// For now break from endless loop only using enter key!
		if (cv::waitKey(1) == 27) break;
	}


	if (cameraDevice->stopCapture() != royale::CameraStatus::SUCCESS) {
		// Camera can not stop recording images (not harmful as this happens at the end!)
		handleTSTATUS(CD_STOP_CAPTURE);
	}


	cout << "Stops running ..." << endl;
}


TSTATUS startUI(cv::Mat& window) {
	// Create window and set background
	window = cv::Mat(cv::Size(WINDOW_WIDTH, WINDOW_HEIGHT), CV_8UC3);
	window = cv::Scalar(157, 170, 161);

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

	// 3) Preview of camera

	return SUCCESS;
}
