#include "carplatenumberdetector.h"

#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/core/utils/filesystem.hpp>

#include "../misc/logger.h"

namespace plate
{
namespace number
{
namespace base
{
CarPlateNumberDetector::CarPlateNumberDetector(const cv::String& imagePath,
                                               const cv::String& pathToHaar) :
    _imagePath(imagePath),
    _pathToHaar(pathToHaar)
{
	if(!cv::utils::fs::exists(imagePath)) {
		CV_ERROR("File '" << imagePath << "' not exists.");
		return;
	}

	_originalImage = cv::imread(_imagePath);
	if(_originalImage.empty()) {
		CV_ERROR("Image '" << _imagePath << "' is not correct.");
		return;
	}

	int width = _originalImage.size().width, height = _originalImage.size().height;
	if(width > 1024) {
		width = 1024;
	}
	if(height > 768) {
		height = 768;
	}

	cv::resize(_originalImage, _originalImage, cv::Size(width, height));

	cv::cvtColor(_originalImage, _originalImage, cv::COLOR_BGR2RGB);
	if(_originalImage.empty()) {
		CV_ERROR("Cannot convert image from BGR to RGB,");
		return;
	}

	if(!_cascadeClassifier.load(_pathToHaar)) {
		CV_ERROR("Cannot load file '" << _pathToHaar << "'.");
		return;
	}
}

cv::String CarPlateNumberDetector::imagePath() const
{
	return _imagePath;
}

cv::Mat CarPlateNumberDetector::detectPlateNumbersOnImage() const
{
	if(!preDetect()) {
		return cv::Mat{};
	}

	cv::Mat img = _originalImage.clone();

	std::vector<cv::Rect> objects;
	_cascadeClassifier.detectMultiScale(img, objects);

	for(const cv::Rect& rect : objects) {
		cv::rectangle(img,
									cv::Point(rect.x, rect.y),
									cv::Point(rect.x + rect.width, rect.y + rect.height),
									cv::Scalar(0, 0, 255),
									5);
	}

	return img;
}

cv::Mat CarPlateNumberDetector::originalImage() const
{
	return _originalImage.clone();
}

bool CarPlateNumberDetector::isValid() const
{
	// TODO: some checks
	return !_originalImage.empty() && !_cascadeClassifier.empty();
}

std::list<cv::Mat> CarPlateNumberDetector::plateNumbers() const
{
	std::list<cv::Mat> plateNumbers;

	if(!preDetect()) {
		return plateNumbers;
	}

	std::vector<cv::Rect> objects;

	cv::Mat img = _originalImage.clone();
	_cascadeClassifier.detectMultiScale(img, objects);

	for(const cv::Rect& rect : objects) {
		// TODO: real size
		cv::Rect realRect(rect.x + 3, rect.y + 6, rect.width - 6, rect.height - 12);
		cv::Mat plateNumberImg(img(realRect));
		if(!plateNumberImg.empty()) {
			plateNumbers.push_back(plateNumberImg);
		}
	}

	return plateNumbers;
}

bool CarPlateNumberDetector::preDetect() const
{
	static bool _prepared = false, result = false;
	if(!_prepared) {
		result = prepareDetector();
		_prepared = false;
	}
	return result;
}

} // namespace base
} // namespace number
} // namespace plate
