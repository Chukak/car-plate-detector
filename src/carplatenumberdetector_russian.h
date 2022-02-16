#ifndef CARPLATENUMBERDETECTOR_RUSSIAN_H
#define CARPLATENUMBERDETECTOR_RUSSIAN_H

#include "base/carplatenumberdetector.h"

namespace plate
{
namespace number
{
class CarPlateNumberDetector_Russian : public base::CarPlateNumberDetector
{
public:
	explicit CarPlateNumberDetector_Russian(const cv::String& imagePath);

protected:
	bool prepareDetector() const final;

private:
	static const cv::String _haarCascadeCarPlateNumberRus;
};
} // namespace number
} // namespace plate

#endif // CARPLATENUMBERDETECTOR_RUSSIAN_H
