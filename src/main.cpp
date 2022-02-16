#include "misc/logger.h"
#include "carplatenumberdetector_russian.h"
#include "misc/ocr.h"

#include <opencv2/highgui.hpp>
#include <map>

static const std::map<int, std::string> TestingDataFiles = {
    {0, "1.png"}, // T111TT97
    {1, "2.png"}, // B007BE777
    {2, "3.png"}, // У202АР125
    {3, "4.png"}, // К574МР77
    {4, "5.png"}, // Т368УУ199
    {5, "6.png"}, //
    {6, "7.jpg"}, // О535РК177
};

cv::String getPathToImage(int index)
{
	cv::String result = SOURCE_DIR;
	result += "/.testing-data/" + TestingDataFiles.at(index);
	return result;
}

int main()
{
	misc::ocr::initGlobalAPI();

	cv::utils::logging::setLogLevel(cv::utils::logging::LOG_LEVEL_VERBOSE);

	plate::number::CarPlateNumberDetector_Russian detector(getPathToImage(2));
	if(detector.isValid()) {
		// Example show the original image with a car plate
		//
		// cv::imshow("Detect car plate numbers", detector.detectPlateNumbersOnImage());
		// cv::waitKey();

		// Example show a car plate number as text
		for(const cv::Mat& img : detector.plateNumbers()) {
			cv::String text = misc::ocr::extractTextFromImage(img);
			std::cout << "FOUND CAR PLATE NUMBER: " << text << " !";
			//	cv::imshow("Plate number", img);

			break;
		}

		// for 4.png, pure car plate number image
		//		{
		//			cv::String text = misc::ocr::extractTextFromImage(detector.originalImage());
		//			std::cout << "Found a car plate number: " << text << std::endl;
		//		}
	}
	// cv::waitKey();

	misc::ocr::destroyGlobalAPI();

	return 0;
}
