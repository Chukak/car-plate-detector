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
  explicit CarPlateNumberDetector();
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
  void processDetectedObjects(const std::vector<cv::Rect>& objects,
                              cv::Mat interestedImage,
                              std::list<cv::Mat>& results,
                              int offsetX = _offsetX,
                              int offsetY = _offsetY,
                              int offsetW = _offsetW,
                              int offsetH = _offsetH) const;
  bool checkImageAsPlateNumber() const;

  private:
  cv::String _imagePath, _pathToHaar;
  cv::Mat _originalImage;
  mutable cv::CascadeClassifier _cascadeClassifier;
  mutable bool _imageIsPlateNumber{false};

  private:
  static const int _offsetX, _offsetY, _offsetW, _offsetH;
};

} // namespace base
} // namespace number
} // namespace plate

#endif // CARPLATENUMBERDETECTOR_H
