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


TSTATUS startUI(cv::Mat& window);


int main() {
	cout << "Starts running ..." << endl;

	TSTATUS stat;


	cout << "Test system on functionality!" << endl;
	if ((stat = checkResolutionRequirements(WINDOW_WIDTH, WINDOW_HEIGHT)) != SUCCESS) {
		cout << "The resolution requirements of " << WINDOW_WIDTH << "x" << WINDOW_HEIGHT << " could not be matched!" << endl;
		cout << "Status: " << stat << endl;
		return -1;
	}


	CameraListener listener;

	unique_ptr<royale::ICameraDevice> cameraDevice;
	if (listener.requestCamera(cameraDevice) != SUCCESS) {
		// TODO: Fehler auswerten (am besten im Listener gespeichert) und ggf mit einfacher GUI reagieren!
		cerr << "No camera detected or receaved nullptr!" << endl
			<< "Maybe camera is not plugged in, drivers are not installed or missing USB permission!" << endl;
		
		return -1;
	}


	if (listener.configureCamera(cameraDevice) != SUCCESS) {
		// TODO: Fehler auswerten (am besten im Listener gespeichert) und ggf mit einfacher GUI reagieren!
		cerr << "Camera could not be configured!" << endl
			<< "Maybe initialization failed or setting of lens parameters or exposure mode!" << endl;
	}


	if (cameraDevice->registerDataListener(&listener) != royale::CameraStatus::SUCCESS) {
		// TODO: Fehler auswerten (am besten im Listener gespeichert) und ggf mit einfacher GUI reagieren!
		cerr << "Error registering data listener" << endl;
		return -5;
	}

	if (cameraDevice->startCapture() != royale::CameraStatus::SUCCESS) {
		// TODO: Fehler auswerten (am besten im Listener gespeichert) und ggf mit einfacher GUI reagieren!
		cerr << "Error starting the capturing" << endl;
		return -6;
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

			/*
			int msgboxid = MessageBox(
				NULL,
				L"You sure u want to quit?",
				L"Confirm quitting!",
				MB_ICONEXCLAMATION | MB_YESNO
			);

			if (msgboxid == IDYES) {
				break;
			}*/
		}


		cvui::update();

		cvui::imshow(WINDOW_NAME, window);


		// For now break from endless loop only using enter key!
		if (cv::waitKey(1) == 27) break;
	}


	if (cameraDevice->stopCapture() != royale::CameraStatus::SUCCESS) {
		cerr << "Error stopping the capturing" << endl;
		return 7;
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
	cvui::button(window, BTN_SAVE_X, BTN_SAVE_Y, BTN_SAVE_WIDTH, BTN_SAVE_HEIGHT, BTN_SAVE_TEXT);

	return SUCCESS;
}