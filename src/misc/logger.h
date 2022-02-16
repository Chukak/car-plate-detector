#ifndef LOGGER_H
#define LOGGER_H

#include <opencv2/core/utils/logger.hpp>

#define CV_TAG cv::utils::logging::internal::getGlobalLogTag()

#define __CV_LOG(macro, ...) macro(CV_TAG, __VA_ARGS__)

#define CV_DEBUG(...) __CV_LOG(CV_LOG_DEBUG, __VA_ARGS__)
#define CV_WARNING(...) __CV_LOG(CV_LOG_WARNING, __VA_ARGS__)
#define CV_ERROR(...) __CV_LOG(CV_LOG_ERROR, __VA_ARGS__)
#define CV_FATAL(...) __CV_LOG(CV_LOG_FATAL, __VA_ARGS__)
#define CV_INFO(...) __CV_LOG(CV_LOG_INFO, __VA_ARGS__)

#endif // LOGGER_H
