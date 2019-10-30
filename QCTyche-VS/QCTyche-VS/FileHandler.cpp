#include <Windows.h>

#include "FileHandler.h"


/// Saves a OpenCV matrix to disk (as an image and as a YML file)
TSTATUS saveMatrix(const std::string& path, const cv::Mat& img, const cv::Mat& mat_float) {
	// 1) Save image
	if (!cv::imwrite(path, img)) {
		// Can not save image to file!
		return FH_SAVE_IMAGE;
	}


	// 2) Save float matrix
	std::string tpath = path;
	tpath.replace(tpath.find(".png", 0), 4, ".yml");

	std::cout << tpath << std::endl;

	try {
		cv::FileStorage file(tpath, cv::FileStorage::WRITE);
		file << YML_SAVE_NAME << mat_float;
		file.release();
	} catch (const std::exception& e) {
		// Can not save float matrix to file
		return FH_SAVE_MATRIX;
	}
	
	return SUCCESS;
}



/// Loads a CV_32F OpenCV matrix from YML file and saves it it given matrix
TSTATUS loadMatrix(const std::string& path, cv::Mat& mat_float) {
	LPCWSTR wpath;
	{
		std::wstring tpath = std::wstring(path.begin(), path.end());
		wpath = tpath.c_str();
	}

	// 1) Check if file exists at given path
	if (GetFileAttributes(wpath) == INVALID_FILE_ATTRIBUTES) {
		// YML file not found at given path
		return FH_LOAD_NOT_FOUND;
	}

	// 2) Read data from file to matrix
	try {
		cv::FileStorage file(path, cv::FileStorage::READ);
		file[YML_SAVE_NAME] >> mat_float;
		file.release();
	} catch (const std::exception& e) {
		// Can not load float matrix from file
		return FH_LOAD_MATRIX;
	}

	return SUCCESS;
}