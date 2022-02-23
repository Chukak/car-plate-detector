#include "carplatenumberdetector_russian.h"
#include "misc/logger.h"
#include "misc/ocr.h"

#include <map>
#include <opencv2/highgui.hpp>

cv::String getPathToImage() {
  cv::String result = SOURCE_DIR;
  result += "/tests/russian/numbers/num_К574МР77.png";
  return result;
}

int main() {
  misc::ocr::initGlobalAPI();

  misc::ocr::internal::constant::__debugMode = true;

  cv::utils::logging::setLogLevel(cv::utils::logging::LOG_LEVEL_VERBOSE);

  plate::number::CarPlateNumberDetector_Russian detector(getPathToImage());
  if(detector.isValid()) {
    // Example show the original image with a car plate
    //
    // cv::imshow("Detect car plate numbers", detector.detectPlateNumbersOnImage());
    // cv::waitKey();

    // Example show a car plate number as text
    for(const cv::Mat& img : detector.plateNumbers()) {
      cv::String text = misc::ocr::extractTextFromImage(img);
      std::cout << "FOUND CAR PLATE NUMBER: " << text << " !" << std::endl;
      break;
    }
  }

  misc::ocr::destroyGlobalAPI();

  return 0;
}
