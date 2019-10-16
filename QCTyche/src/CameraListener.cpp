//
// Created by thahnen on 16.10.19.
//

#include "CameraListener.h"


void CameraListener::setLensParameters(const royale::LensParameters& lensParameters) {
    cameraMatrix = (cv::Mat1d(3, 3)
                        << lensParameters.focalLength.first,
                            0,
                            lensParameters.principalPoint.first,
                            0,
                            lensParameters.focalLength.second,
                            lensParameters.principalPoint.second,
                            0,
                            0,
                            1
                    );

    distortionCoefficients = (cv::Mat1d(1, 5)
                                << lensParameters.distortionRadial[0],
                                    lensParameters.distortionRadial[1],
                                    lensParameters.distortionTangential.first,
                                    lensParameters.distortionTangential.second,
                                    lensParameters.distortionRadial[2]
                            );
}


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