//
// Created by thahnen on 16.10.19.
//

#pragma once

#include <mutex>
#include <royale.hpp>
#include <opencv2/opencv.hpp>

#include "Status.h"


class CameraListener : public royale::IDepthDataListener {
public:
	/**
	 *	Requests a camera device from the list provided by the camera manager
	 *
	 *	@param device			where the camera should be stored
	 *	@return					SUCCESS if the resolution matches the requirements otherwise an error
	 */
	TSTATUS requestCamera(std::unique_ptr<royale::ICameraDevice>& device);


	/**
	 *	Requests a specific camera device from the provided list
	 *
	 *	@param device			where the camera should be stored
	 *	@param lstitem			which camera should be used, starting with 0
	 *	@return					SUCCESS if the resolution matches the requirements otherwise an error
	 */
	TSTATUS requestSpecificCamera(std::unique_ptr<royale::ICameraDevice>& device, int lstitem);


	/**
	 *	Configures the camera for capturing frames
	 *
	 *	@param device			which camera should be configured
	 *	@return					SUCCESS if the resolution matches the requirements otherwise an error
	 */
	TSTATUS configureCamera(std::unique_ptr<royale::ICameraDevice>& device);


	/**
	 *  Handler executed when receiving new data from camera (every frame)
	 *
	 *  @param data             the received data
	 */
	void onNewData(const royale::DepthData* data) override;


	/// Returns the newest depth image, conversion using type
	cv::Mat getNewestDepthImage() { return depthImage; }
	cv::Mat getNewestDepthImage(int type);

	/// Returns the newest grayscale image, conversion using type
	cv::Mat getNewestGrayscaleImage() { return grayImage; }
	cv::Mat getNewestGrayscaleImage(int type);
private:
	/**
	 *  Setting the camera lens' parameters for getting an optimal image
	 *  Must be run before the camera starts reading data!
	 *
	 *  @param lensParameters   the specific parameters which should be used
	 */
	void setLensParameters(const royale::LensParameters& lensParameters);


	std::mutex flagMutex;

	cv::Mat cameraMatrix;
	cv::Mat distortionCoefficients;

	cv::Mat depthImage;
	cv::Mat grayImage;
};