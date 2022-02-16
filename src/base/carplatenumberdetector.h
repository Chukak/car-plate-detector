#ifndef CARPLATENUMBERDETECTOR_H
#define CARPLATENUMBERDETECTOR_H

#include <list>
#include <opencv2/core/cvstd.hpp>
#include <opencv2/core/mat.hpp>
#include <opencv2/objdetect.hpp>

namespace plate {
namespace number {
namespace base {
/**
 * @brief The CarPlateNumberDetector class
 */
class CarPlateNumberDetector {
  public:
  explicit CarPlateNumberDetector(const cv::String& imagePath, const cv::String& pathToHaar);

  cv::String imagePath() const;
  cv::Mat detectPlateNumbersOnImage() const;
  cv::Mat originalImage() const;
  virtual bool isValid() const;

  std::list<cv::Mat> plateNumbers() const;

  protected:
  virtual bool prepareDetector() const = 0;

  private:
  bool preDetect() const;

  private:
  cv::String _imagePath, _pathToHaar;
  cv::Mat _originalImage;
  mutable cv::CascadeClassifier _cascadeClassifier;
};

} // namespace base
} // namespace number
} // namespace plate

#endif // CARPLATENUMBERDETECTOR_H
