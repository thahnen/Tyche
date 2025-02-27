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
	 *
	 *	TODO: see requestSpecificCamera function!
	 */
	TSTATUS requestCamera(std::unique_ptr<royale::ICameraDevice>& device);


	/**
	 *	Requests a specific camera device from the provided list
	 *
	 *	@param device			where the camera should be stored
	 *	@param lstitem			which camera should be used, starting with 0
	 *	@return					SUCCESS if the resolution matches the requirements otherwise an error
	 *	
	 *	TODO: inform the user if more than one camera is connected, maybe give him a selection!
	 *	TODO: maybe preview an image to let the user select?
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


	/// Returns the newest depth image, conversion using type (without conversion the type is cv::CV_32F)
	cv::Mat getNewestDepthImage() { return depthImage; }
	cv::Mat getNewestDepthImage(int type);

	/// Returns the newest grayscale image, conversion using type (without conversion the type is cv::CV_32F)
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

	/// Mutex for updating the images per frame
	std::mutex flagMutex;

	/// The matrices derived from the camera lens parameters
	cv::Mat cameraMatrix;
	cv::Mat distortionCoefficients;

	/// The two images produced by the camera
	cv::Mat depthImage;
	cv::Mat grayImage;
};