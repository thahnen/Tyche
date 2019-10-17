//
// Created by thahnen on 16.10.19.
//

#pragma once

#include <mutex>
#include <royale.hpp>
#include <opencv2/opencv.hpp>


class CameraListener : public royale::IDepthDataListener {
public:
	/**
	 *  Setting the camera lens' parameters for getting an optimal image
	 *  Must be run
	 *
	 *  @param lensParameters   the specific parameters which should be used
	 */
	void setLensParameters(const royale::LensParameters& lensParameters);


	/**
	 *  Listener executed when receiving new data from camera (every frame)
	 *
	 *  @param data             the received data
	 */
	void onNewData(const royale::DepthData* data) override;

private:
	std::mutex flagMutex;

	cv::Mat cameraMatrix;
	cv::Mat distortionCoefficients;

	cv::Mat depthImage;
	cv::Mat grayImage;
};