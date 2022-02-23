#include "carplatenumberdetector_russian.h"
#include "misc/logger.h"

#include <opencv2/core/utils/filesystem.hpp>

namespace plate {
namespace number {
const cv::String CarPlateNumberDetector_Russian::_haarCascadeCarPlateNumberRus(HAARCASCADE_CAR_PLATE_NUMBER_RUS);

CarPlateNumberDetector_Russian::CarPlateNumberDetector_Russian() : base::CarPlateNumberDetector() {
}

CarPlateNumberDetector_Russian::CarPlateNumberDetector_Russian(const cv::String& imagePath) :
    base::CarPlateNumberDetector(imagePath, _haarCascadeCarPlateNumberRus) {
}

bool CarPlateNumberDetector_Russian::prepareDetector() const {
  bool prepared = false;
  if(!(prepared = cv::utils::fs::exists(_haarCascadeCarPlateNumberRus))) {
    CV_ERROR("No such file '" << _haarCascadeCarPlateNumberRus << "'.");
  }
  return prepared;
}
} // namespace number
} // namespace plate
