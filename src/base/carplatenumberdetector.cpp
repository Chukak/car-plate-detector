#include "../misc/logger.h"
#include "carplatenumberdetector.h"

#include <opencv2/core/utils/filesystem.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>

namespace plate {
namespace number {
namespace base {

const int CarPlateNumberDetector::_offsetX = 3, CarPlateNumberDetector::_offsetY = 6, CarPlateNumberDetector::_offsetW = 6,
          CarPlateNumberDetector::_offsetH = 12;

CarPlateNumberDetector::CarPlateNumberDetector(const cv::String& imagePath, const cv::String& pathToHaar) :
    _imagePath(imagePath), _pathToHaar(pathToHaar) {
  if(!_cascadeClassifier.load(_pathToHaar)) {
    CV_ERROR("Cannot load file '" << _pathToHaar << "'.");
    return;
  }

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
  width = width > 1024 ? 1024 : width;
  height = height > 768 ? 768 : height;

  if(checkImageAsPlateNumber()) {
    width = 512;
    height = 110;
  }

  cv::resize(_originalImage, _originalImage, cv::Size(width, height));

  cv::cvtColor(_originalImage, _originalImage, cv::COLOR_BGR2RGB);
  if(_originalImage.empty()) {
    CV_ERROR("Cannot convert image from BGR to RGB,");
    return;
  }
}

cv::String CarPlateNumberDetector::imagePath() const {
  return _imagePath;
}

cv::Mat CarPlateNumberDetector::detectPlateNumbersOnImage() const {
  if(!preDetect()) { return cv::Mat{}; }

  cv::Mat img = _originalImage.clone();

  if(!checkImageAsPlateNumber()) {
    std::vector<cv::Rect> objects;
    _cascadeClassifier.detectMultiScale(img, objects);

    for(const cv::Rect& rect : objects) {
      cv::rectangle(img, cv::Point(rect.x, rect.y), cv::Point(rect.x + rect.width, rect.y + rect.height), cv::Scalar(0, 0, 255), 5);
    }
  }

  return img;
}

cv::Mat CarPlateNumberDetector::originalImage() const {
  return _originalImage.clone();
}

bool CarPlateNumberDetector::isValid() const {
  // TODO: some checks
  return !_originalImage.empty() && !_cascadeClassifier.empty();
}

std::list<cv::Mat> CarPlateNumberDetector::plateNumbers() const {
  std::list<cv::Mat> plateNumbers;

  if(!preDetect()) { return plateNumbers; }

  if(checkImageAsPlateNumber()) {
    plateNumbers.push_back(_originalImage.clone());
  } else {
    std::vector<cv::Rect> objects;

    cv::Mat img = _originalImage.clone();
    _cascadeClassifier.detectMultiScale(img, objects);

    processDetectedObjects(objects, img, plateNumbers);
  }

  return plateNumbers;
}

bool CarPlateNumberDetector::preDetect() const {
  static bool _prepared = false, result = false;
  if(!_prepared) {
    result = prepareDetector();
    _prepared = false;
  }
  return result;
}

void CarPlateNumberDetector::processDetectedObjects(const std::vector<cv::Rect>& objects,
                                                    cv::Mat interestedImage,
                                                    std::list<cv::Mat>& results,
                                                    int offsetX,
                                                    int offsetY,
                                                    int offsetW,
                                                    int offsetH) const {
  for(const cv::Rect& rect : objects) {
    // TODO: real size
    cv::Rect realRect(rect.x + offsetX, rect.y + offsetY, rect.width - offsetW, rect.height - offsetH);
    cv::Mat plateNumberImg(interestedImage(realRect));

    if(!plateNumberImg.empty()) { results.push_back(plateNumberImg); }
  }
}

bool CarPlateNumberDetector::checkImageAsPlateNumber() const {
  if(!_imageIsPlateNumber) {
    cv::Mat img = cv::Mat(cv::Size(_originalImage.cols + _offsetX * 20, _originalImage.rows + _offsetY * 15), _originalImage.type());
    _originalImage.copyTo(
        img(cv::Rect((_offsetX * 20) / 2, (_offsetY * 15) / 2, img.size().width - _offsetX * 20, img.size().height - _offsetY * 15)));

    std::vector<cv::Rect> objects;
    _cascadeClassifier.detectMultiScale(img, objects);

    if(objects.size() == 1) {
      cv::Rect rect = objects.at(0);

      _imageIsPlateNumber = (rect.width >= _originalImage.size().width && rect.height >= _originalImage.size().height) ||
          (rect.width + _offsetX * 3 >= _originalImage.size().width && rect.height + _offsetY * 2 >= _originalImage.size().height);
    }
  }

  return _imageIsPlateNumber;
}

} // namespace base
} // namespace number
} // namespace plate
