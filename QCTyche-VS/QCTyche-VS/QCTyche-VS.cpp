// QCTyche-VS.cpp : Diese Datei enthält die Funktion "main". Hier beginnt und endet die Ausführung des Programms.
//

#include <iostream>
#include <Windows.h>
#include <opencv2/opencv.hpp>
#include <royale.hpp>

#define CVUI_IMPLEMENTATION
#include "cvui.h"

#include "CameraListener.h"

using namespace std;


int main() {
	cout << "Starts running ..." << endl;


	CameraListener listener;

	unique_ptr<royale::ICameraDevice> cameraDevice;
	if (listener.requestCamera(cameraDevice) < 0) {
		cerr << "No camera detected or receaved nullptr!" << endl
			<< "Maybe camera is not plugged in, drivers are not installed or missing USB permission!" << endl;
		
		return -1;
	}


	if (listener.configureCamera(cameraDevice) < 0) {
		cerr << "Camera could not be configured!" << endl
			<< "Maybe initialization failed or setting of lens parameters or exposure mode!" << endl;
	}


	if (cameraDevice->registerDataListener(&listener) != royale::CameraStatus::SUCCESS) {
		cerr << "Error registering data listener" << endl;
		return -5;
	}

	if (cameraDevice->startCapture() != royale::CameraStatus::SUCCESS) {
		cerr << "Error starting the capturing" << endl;
		return -6;
	}

	/// Test sleeping!
	Sleep(1000);


	for (;;) {
		cv::Mat current_grayscale = listener.getNewestGrayscaleImage(CV_8UC1);
		cv::Mat current_depth = listener.getNewestDepthImage(CV_8UC1);

		cv::imshow("Grayscale", current_grayscale);
		cv::imshow("Depth", current_depth);

		// For now break from endless loop only using enter key!
		if (cv::waitKey(1) == 43) break;
	}


	if (cameraDevice->stopCapture() != royale::CameraStatus::SUCCESS) {
		cerr << "Error stopping the capturing" << endl;
		return 7;
	}


	cout << "Stops running ..." << endl;
}