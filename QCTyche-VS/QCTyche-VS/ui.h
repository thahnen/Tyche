#pragma once

#include <opencv2/opencv.hpp>
#include "cvui.h"
#include "CameraListener.h"

#define WINDOW_NAME		"QCTyche"
#define WINDOW_WIDTH	640
#define WINDOW_HEIGHT	480


class ui {
public:
	ui(cv::Mat nWindow, CameraListener& nListener);


	int initialize();

	int loop();

private:
	cv::Mat window;
	CameraListener& listener;
};

