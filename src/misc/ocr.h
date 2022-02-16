#ifndef UTILS_H
#define UTILS_H

#include <opencv2/core/cvstd.hpp>
#include <opencv2/core/mat.hpp>
#include <tesseract/baseapi.h>

namespace misc
{
namespace ocr
{
using TesseractAPIPtr = std::shared_ptr<tesseract::TessBaseAPI>;
/**
 * @brief initGlobalAPI
 * Initializes a global TesseractOCR API.
 * @return TesseractOCR API.
 */
TesseractAPIPtr initGlobalAPI();
/**
 * @brief destroyGlobalAPI
 * Destroys a global TesseractAPI.
 */
void destroyGlobalAPI();
/**
 * @brief globalAPI
 * @return TesseractOCR API.
 */
TesseractAPIPtr globalAPI();
/**
 * @brief extractTextFromImage
 * Extract a text from the image. Text is a russian car plate number.
 * @param mat The image
 * @param api TesseractOCR API
 * @return a text or an empty string.
 */
cv::String extractTextFromImage(const cv::Mat& mat, TesseractAPIPtr api = globalAPI());

/**
 * @namespace internal
 * Contains functions and variables to work with images. Use it if you really need to.
 */
namespace internal
{
struct constant
{
	/**
	 * @brief DF_imageMaxSideWidth, DF_imageMaxSideHeight
	 * Maximal width or height value to resize. By default: width is 130, height is 180.
	 * \see searchTextOnDifferentSizes(...)
	 */
	static int DF_imageMaxSideWidth, DF_imageMaxSideHeight;
	/**
	 * @brief BLUR_MaxLevel
	 * Maximal image blur level. By defaut is 18.
	 * \see searchTextOnBluredImage(...)
	 */
	static int BLUR_MaxLevel;
	/**
	 * @brief SZ_imageWidth, SZ_imageHeight
	 * Size of prepared to process image. By default: width is 520, height is 112.
	 */
	static int SZ_imageWidth, SZ_imageHeight;
	/**
	 * Undocumented variables
	 */
	static double LIMIT_imageMaxWidthCoef, LIMIT_imageMaxHeightCoef,
			LIMIT_imageMinWidthCoef, LIMIT_imageMinHeightCoef, LIMIT_imageMinWidthInternalCoef,
			LIMIT_imageMinHeightInternalCoef;
	/**
	 * Whitelist variables. Undocumented.
	 */
	const static cv::String WL_characters, WL_digits;

	static bool __debugMode;
};
/**
 * @brief searchTextOnBluredImage
 * Searches a text on a blured image, while startLevel < constants::BLUR_MaxLevel or text
 * will found. The passed image must be a deep copy.
 * @param tessApi TesseractOCR API
 * @param mat Deep copy of an image
 * @param startLevel Starting blur level
 * @return a text or an empty string.
 */
cv::String searchTextOnBluredImage(std::shared_ptr<tesseract::TessBaseAPI> tessApi,
                                   cv::Mat mat,
                                   int startLevel = 5);
/**
 * @brief The DF_SideSize enum
 * Types to resize image by side.
 * \see searchTextOnDifferentSizes(...)
 */
enum DF_SideSize
{
	DF_Width,      //! Only width
	DF_Height,     //! Only Height
	DF_WidthHeight //! Both of them
};
/**
 * @brief searchTextOnDifferentSizes
 * Searches a text on an image with different sizes, while width <
 * constants::DF_imageMaxSideWidth, height < DF_imageMaxSideHeight or text will found. The
 * passed image must be a deep copy.
 * @param tessApi TesseractOCR API
 * @param mat Deep copy of an image
 * @param width Starting iamge width
 * @param height Starting image height
 * @param side Side to resize
 * @param upCoefVal Coefficient to imcrement size of side
 * @return a text or an empty string.
 */
cv::String searchTextOnDifferentSizes(std::shared_ptr<tesseract::TessBaseAPI> tessApi,
                                      cv::Mat mat,
                                      int width,
                                      int height,
                                      DF_SideSize side,
                                      int upCoefVal = 10);
/**
 * @brief isInternalContour
 * Checks if contour is internal.
 * @param contours List of contours
 * @param contour Contour to check
 * @return contour is enternal.
 */
bool isInternalContour(const std::vector<std::vector<cv::Point>>& contours,
                       const std::vector<cv::Point>& contour);
/**
 * @brief uniqueConsours
 * Removes duplicates.
 * @param contours List of contours
 */
void uniqueConsours(std::vector<std::vector<cv::Point>>& contours);
/**
 * @brief textFromImage
 * Get text from an image using TesseractOCR API.
 * @param tessApi TesseractOCR API
 * @param mat an image (no deep copy!)
 * @return a text or an empty string.
 */
cv::String textFromImage(std::shared_ptr<tesseract::TessBaseAPI> tessApi, cv::Mat mat);

namespace text
{
/**
 * @brief utf82Str
 * Copy a text from a char array to the cv::String. Text is a character or digit.
 * @param utf8
 * @param len
 * @return text as cv::String.
 */
cv::String utf82Str(const char* utf8, size_t len);
} // namespace text
} // namespace internal
} // namespace ocr
} // namespace misc

#endif // UTILS_H
