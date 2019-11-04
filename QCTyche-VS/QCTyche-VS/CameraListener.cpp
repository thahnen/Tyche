//
// Created by thahnen on 16.10.19.
//

#include "CameraListener.h"
#include "Logger.h"


///	Requests a camera device from the list provided by the camera manager
TSTATUS CameraListener::requestCamera(std::unique_ptr<royale::ICameraDevice>& device) {
	return requestSpecificCamera(device, 0);
}


///	Requests a specific camera device from the provided list
TSTATUS CameraListener::requestSpecificCamera(std::unique_ptr<royale::ICameraDevice>& device, int lstitem) {
	royale::CameraManager manager;
	royale::Vector<royale::String> camlist(manager.getConnectedCameraList());

	if (camlist.empty())
		return CL_LIST_EMPTY;

	if (lstitem >= camlist.size())
		return CL_LIST_OOB;

	device = manager.createCamera(camlist[lstitem]);
	if (device == nullptr)
		return CL_LIST_NULLPTR;

	// Additionally test if there is more than one camera (maybe the wrong one is used)
	TSTATUS stat;
	if (camlist.size() > 1)	stat = CL_MORE_CAMS;
	else					stat = SUCCESS;

	camlist.clear();
	return stat;
}


/// Configures the camera for capturing frames
TSTATUS CameraListener::configureCamera(std::unique_ptr<royale::ICameraDevice>& device) {
	// TODO: try-catch darum, wenn Kamera nicht angeschlossen!
	try {
		if (device->initialize() != royale::CameraStatus::SUCCESS)
			return CL_CAMERA_INIT;
	} catch (const std::exception&) {
		// Lesezugriffsverletzung, weil Kamera nicht angeschlossen war!
		return CL_CAMERA_INIT_RAV;
	}

	royale::LensParameters lensParameters;
	if (device->getLensParameters(lensParameters) != royale::CameraStatus::SUCCESS)
		return CL_LENS_PARAM;

	setLensParameters(lensParameters);

	if (device->setExposureMode(royale::ExposureMode::AUTOMATIC) != royale::CameraStatus::SUCCESS)
		return CL_EXPOSURE_MODE;

	return SUCCESS;
}


/// Handler executed when receiving new data from camera (every frame)
void CameraListener::onNewData(const royale::DepthData* data) {
	std::lock_guard<std::mutex> lock(flagMutex);

	depthImage.create(cv::Size(data->width, data->height), CV_32FC1);
	depthImage = 0;
	//depthImage = cv::Mat::zeros(cv::Size(data->width, data->height), CV_32FC1);

	grayImage.create(cv::Size(data->width, data->height), CV_32FC1);
	grayImage = 0;
	//grayImage = cv::Mat::zeros(cv::Size(data->width, data->height), CV_32FC1);

	int k = 0;
	for (int y = 0; y < depthImage.rows; y++) {
		for (int x = 0; x < depthImage.cols; x++) {
			auto currentPoint = data->points.at(k);
			if (currentPoint.depthConfidence > 0) {
				depthImage.at<float>(y, x) = currentPoint.z;
				grayImage.at<float>(y, x) = currentPoint.grayValue;
			}
			k++;
		}
	}

	cv::Mat temp = depthImage.clone();
	cv::undistort(temp, depthImage, cameraMatrix, distortionCoefficients);

	temp = grayImage.clone();
	cv::undistort(temp, grayImage, cameraMatrix, distortionCoefficients);
}


/// Returns the newest depth image converted to given type
// TODO: implement
cv::Mat CameraListener::getNewestDepthImage(int type) {
	// Kontrastspreitzung
	cv::Mat mask;
	cv::compare(depthImage, 0, mask, cv::CMP_GT);
	double min, max, scale;
	cv::minMaxLoc(depthImage, &min, &max, 0, 0, mask);

	cv::Mat conv;

	switch (type) {
	case CV_8UC1:
		conv = cv::Mat::zeros(depthImage.rows, depthImage.cols, CV_8UC1);

		// Skalierung
		scale = 255 / (min - max);
		cv::convertScaleAbs(depthImage, conv, scale, (-min * scale));
		break;
	default:
		log(LERROR, "CameraListerner::getNewestDepthImage - Given type not allowed!");
	}

	return conv;
}


/// Returns the newest grayscale image converted to given type
// TODO: implement
cv::Mat CameraListener::getNewestGrayscaleImage(int type) {
	// Kontrastspreitzung
	cv::Mat mask;
	cv::compare(grayImage, 0, mask, cv::CMP_GT);
	double min, max, scale;
	cv::minMaxLoc(grayImage, &min, &max, 0, 0, mask);

	cv::Mat conv;

	switch (type) {
	case CV_8UC1:
		conv = cv::Mat::zeros(grayImage.rows, grayImage.cols, CV_8UC1);

		// Skalierung
		scale = 255 / (max - min);
		cv::convertScaleAbs(grayImage, conv, scale);
		break;
	default:
		log(LERROR, "CameraListerner::getNewestGrayscaleImage - Given type not allowed!");
	}

	return conv;
}


/// Sets the lens parameters of the camera
void CameraListener::setLensParameters(const royale::LensParameters& lensParameters) {
	cameraMatrix = (cv::Mat1d(3, 3)
		<< lensParameters.focalLength.first, 0, lensParameters.principalPoint.first,
			0, lensParameters.focalLength.second, lensParameters.principalPoint.second,
			0, 0, 1
		);

	distortionCoefficients = (cv::Mat1d(1, 5)
		<< lensParameters.distortionRadial[0],
			lensParameters.distortionRadial[1],
			lensParameters.distortionTangential.first,
			lensParameters.distortionTangential.second,
			lensParameters.distortionRadial[2]
		);
}
