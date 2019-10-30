#pragma once

#include <string>
#include <opencv2/opencv.hpp>

#include "Status.h"

#define YML_SAVE_NAME	"Matrix"


/**
 *	Saves a OpenCV matrix to disk (as an image and as a YML file)
 *
 *	@param path				the path to save the matrix to
 *	@param img				the image matrix which has to be saved
 *	@param mat_float		the CV_32F matrix which has to be saved
 *	@return					SUCCESS if the matrix was saved to image and binary otherwise an error
 */
TSTATUS saveMatrix(const std::string& path, const cv::Mat& img, const cv::Mat& mat_float);


/**
 *	Loads a CV_32F OpenCV matrix from YML file and saves it it given matrix
 *
 *	@param path				the path to load the matrix from
 *	@param mat_float		the CV32F matrix the file data should be saved to
 *	@return					SUCCESS if the matrix could be loaded from file otherwise an error
 */
TSTATUS loadMatrix(const std::string& path, cv::Mat& mat_float);