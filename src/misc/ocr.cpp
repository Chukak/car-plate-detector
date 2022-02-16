#include "ocr.h"
#include "logger.h"
#include <opencv2/text/ocr.hpp>

#include <opencv2/imgproc.hpp>
#include <opencv2/photo.hpp>

#include <opencv2/highgui.hpp>
#include <iostream>
#include <boost/locale.hpp>

namespace misc
{
namespace ocr
{
std::shared_ptr<tesseract::TessBaseAPI> _api(new tesseract::TessBaseAPI);

TesseractAPIPtr initGlobalAPI()
{
	if(_api->Init(NULL, "rus+eng", tesseract::OEM_LSTM_ONLY) != 0) {
		CV_WARNING("Failed to init a TesseractOCR API instance.");
	}
	// api->SetVariable("tessedit_enable_doc_dict", "0");
	_api->SetVariable("load_system_dawg", "0");
	_api->SetVariable("load_freq_dawg", "0");

	// TODO: does not work on some images
	_api->SetVariable("tessedit_do_invert", "0");
	_api->SetVariable("textord_tabfind_vertical_text", "0");
	_api->SetPageSegMode(tesseract::PSM_SINGLE_CHAR);

	return globalAPI();
}

void destroyGlobalAPI()
{
	_api->End();
}

TesseractAPIPtr globalAPI()
{
	return _api;
}

cv::String extractTextFromImage(const cv::Mat& mat, TesseractAPIPtr api)
{
	cv::String result;

	cv::Mat img = mat.clone();

	cv::resize(img,
						 img,
						 cv::Size(internal::constant::SZ_imageWidth,
											internal::constant::SZ_imageHeight));
	cv::cvtColor(img, img, cv::COLOR_RGBA2GRAY);
	cv::threshold(img, img, 0, 255, cv::THRESH_BINARY | cv::THRESH_OTSU);
	cv::medianBlur(img, img, 3);
	cv::blur(img, img, cv::Size(3, 3));

	cv::Canny(img, img, 100, 200);

	std::vector<std::vector<cv::Point>> conts;
	std::vector<cv::Vec4i> hierarcy;
	cv::findContours(img, conts, hierarcy, cv::RETR_TREE, cv::CHAIN_APPROX_SIMPLE);

	std::vector<cv::Rect> usedRects;

	std::sort(conts.begin(),
						conts.end(),
						[](const std::vector<cv::Point>& lhs, const std::vector<cv::Point>& rhs) {
							cv::Rect a(cv::boundingRect(lhs)), b(cv::boundingRect(rhs));
							return a.x < b.x;
						});

	if(internal::constant::__debugMode) {
		cv::imshow("Prepared", img);
		cv::waitKey();
	}

	cv::Mat temp(cv::Size(img.cols, img.rows), 0, cv::Scalar(5));
	for(auto start = conts.begin(); start != conts.end();) {
		cv::Rect r = cv::boundingRect(*start);
		bool isInternal = internal::isInternalContour(conts, *start);

		if(r.width > img.size().width * internal::constant::LIMIT_imageMaxWidthCoef ||
			 r.height > img.size().height * internal::constant::LIMIT_imageMaxHeightCoef ||
			 (!isInternal &&
				r.width < internal::constant::LIMIT_imageMinWidthCoef * img.size().width) ||
			 (!isInternal &&
				r.height < internal::constant::LIMIT_imageMinHeightCoef * img.size().height) ||
			 (isInternal && r.width < internal::constant::LIMIT_imageMinWidthInternalCoef *
																		img.size().width) ||
			 (isInternal && r.height < internal::constant::LIMIT_imageMinHeightInternalCoef *
																		 img.size().height) ||
			 r.width / 2 > r.height || //
			 r.x == 0 || r.y == 0) {
			conts.erase(start);
			continue;
		}

		cv::fillPoly(temp,
								 std::vector<std::vector<cv::Point>>{*start},
								 cv::Scalar(255, 255, 255));
		++start;
	}

	internal::uniqueConsours(conts);

	for(auto start = conts.begin(); start != conts.end();) {
		cv::Rect r = cv::boundingRect(*start);
		if(internal::isInternalContour(conts, *start)) {
			cv::fillPoly(temp,
									 std::vector<std::vector<cv::Point>>{*start},
									 cv::Scalar(0, 0, 0));

			conts.erase(start);
			continue;
		}
		++start;
	}

	if(internal::constant::__debugMode) {
		cv::imshow("Only car plate number", temp);
		cv::waitKey();
	}

	int index = 0;
	for(auto con : conts) {
		cv::Rect r = cv::boundingRect(con);

		cv::Mat contImg = temp(cv::Rect(r.x >= 5 ? r.x - 5 : r.x,
																		r.y >= 5 ? r.y - 5 : r.y,
																		r.width + (r.x >= 5 ? 10 : 0),
																		r.height + (r.y >= 5 ? 10 : 0)));

		cv::Size contImgSize(100, 100);

		cv::resize(contImg, contImg, contImgSize);
		cv::Mat contImgKern = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(5, 5));
		cv::dilate(contImg, contImg, contImgKern, cv::Point(-1, -1), 1);
		// cv::erode(contImg, contImg, contImgKern, cv::Point(-1, -1), 1);
		// cv::morphologyEx(contImg, contImg, cv::MORPH_CLOSE, contImgKern);

		//

		cv::bitwise_not(contImg, contImg);

		if(internal::constant::__debugMode) {
			cv::imshow("Contour", contImg);
			cv::waitKey();
		}

		cv::Mat origContImg = contImg.clone();

		api->Clear();

		if(index == 0 || index == 4 || index == 5) {
			api->SetVariable("tessedit_char_whitelist",
											 internal::constant::WL_characters.data());
		} else {
			api->SetVariable("tessedit_char_whitelist", internal::constant::WL_digits.data());
		}
		api->SetVariable("user_defined_dpi", "300");

		cv::String res = internal::searchTextOnDifferentSizes(api,
																													contImg.clone(),
																													contImg.size().width,
																													contImg.size().height,
																													internal::DF_Height);
		if(res.empty()) {
			res = internal::searchTextOnDifferentSizes(api,
																								 contImg.clone(),
																								 contImg.size().width,
																								 contImg.size().height,
																								 internal::DF_Width);
		}

		if(res.empty()) {
			res = internal::searchTextOnBluredImage(api, contImg.clone());
		}

		if(res.empty()) {
			res = " ";
		}

		result += res;

		api->Clear();

		usedRects.push_back(r);
		++index;
	}

	static const boost::locale::generator gen;
	// TODO: utf8 or UTF-8
	static const std::locale _loc = gen.generate("ru_RU.utf8");

	result = boost::locale::to_upper(result, _loc);
	return result;
}

namespace internal
{
int constant::DF_imageMaxSideWidth = 130;
int constant::DF_imageMaxSideHeight = 180;

int constant::BLUR_MaxLevel = 18;

int constant::SZ_imageWidth = 520, constant::SZ_imageHeight = 112;

double constant::LIMIT_imageMaxWidthCoef = 0.22,
       constant::LIMIT_imageMaxHeightCoef = 0.80,
       constant::LIMIT_imageMinWidthCoef = 0.03 /* 0.01 */,
       constant::LIMIT_imageMinHeightCoef = 0.2,
       constant::LIMIT_imageMinWidthInternalCoef = 0.010,
       constant::LIMIT_imageMinHeightInternalCoef = 0.09;

bool constant::__debugMode = true;

const cv::String constant::WL_characters("аАвВеЕкКмМнНоОрРсСтТуУхХ"), constant::WL_digits(
                                                                          "0123456789");

cv::String searchTextOnBluredImage(std::shared_ptr<tesseract::TessBaseAPI> tessApi,
                                   cv::Mat mat,
                                   int startLevel)
{
	cv::String result;
	do {
		if(startLevel > constant::BLUR_MaxLevel) {
			break;
		}

		while((startLevel * startLevel) % 2 == 0) {
			++startLevel;
		}

		result = textFromImage(tessApi, mat);

		cv::GaussianBlur(mat, mat, cv::Size(startLevel, startLevel), 0);
		++startLevel;
	} while(result.empty());

	return result;
}

cv::String searchTextOnDifferentSizes(std::shared_ptr<tesseract::TessBaseAPI> tessApi,
                                      cv::Mat mat,
                                      int width,
                                      int height,
                                      DF_SideSize side,
                                      int upCoefVal)
{
	cv::String result;
	do {
		if(((side == DF_Width || side == DF_WidthHeight) &&
				width > constant::DF_imageMaxSideWidth) ||
			 height > constant::DF_imageMaxSideHeight) {
			break;
		}

		cv::resize(mat, mat, cv::Size(width, height));

		cv::Mat blured;
		cv::GaussianBlur(mat, blured, cv::Size(3, 3), 0);
		result = textFromImage(tessApi, blured);

		if(side == DF_Width || side == DF_WidthHeight) {
			width += upCoefVal;
		}
		if(side == DF_Height || side == DF_WidthHeight) {
			height += upCoefVal;
		}

	} while(result.empty());

	return result;
}

bool isInternalContour(const std::vector<std::vector<cv::Point>>& contours,
											 const std::vector<cv::Point>& contour)
{
	cv::Rect bContour = cv::boundingRect(contour);
	std::remove_reference_t<decltype(contours)>::const_iterator found =
			std::find_if(contours.cbegin(),
									 contours.cend(),
									 [bContour](const std::vector<cv::Point>& po) {
										 cv::Rect rect = cv::boundingRect(po);
										 return rect != bContour && rect.contains(bContour.tl());
									 });
	return contours.cend() != found;
}

void uniqueConsours(std::vector<std::vector<cv::Point>>& contours)
{
	contours.erase(std::unique(contours.begin(),
														 contours.end(),
														 [](const std::vector<cv::Point>& lhs,
																const std::vector<cv::Point>& rhs) {
															 cv::Rect a(cv::boundingRect(lhs)),
																	 b(cv::boundingRect(rhs));
															 return a == b;
														 }),
								 contours.end());
}

cv::String textFromImage(std::shared_ptr<tesseract::TessBaseAPI> tessApi, cv::Mat mat)
{
	cv::String result;

	tessApi->Clear();
	tessApi->TesseractRect(mat.data, mat.channels(), mat.step1(), 0, 0, mat.cols, mat.rows);
	char* text = tessApi->GetUTF8Text();
	size_t textLen = std::strlen(text);
	if(textLen != 0 && text[0] != ' ') {
		result = text::utf82Str(text, textLen);
	}
	delete[] text;

	return result;
}

namespace text
{
cv::String utf82Str(const char* utf8, size_t len)
{
	cv::String result;
	size_t lenResult = 0;
	if(len > 1 && utf8[0] != ' ' && utf8[0] != '\0') {
		lenResult = 1;
		if(len > 2 && utf8[1] != ' ' && utf8[1] != '\0') {
			lenResult = 2;
		}
	}
	if(lenResult > 0) {
		result = cv::String(lenResult, ' ');
		std::strncpy(&result[0], utf8, lenResult);
	}
	return result;
}
} // namespace text
} // namespace internal
} // namespace ocr
} // namespace misc
