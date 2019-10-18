#pragma once

#include <opencv2/opencv.hpp>
#include "cvui.h"
#include "CameraListener.h"

#define WINDOW_NAME		"QCTyche"


class ui {
public:
	ui(cv::Mat nWindow, CameraListener& nListener);


	int initialize();

	int loop();

private:
	cv::Mat window;
	CameraListener& listener;
};

