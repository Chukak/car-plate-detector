#include "carplatenumberdetector_russian.h"
#include "misc/logger.h"
#include "misc/ocr.h"

#include <QAbstractButton>
#include <QApplication>
#include <QFileDialog>
#include <QMessageBox>
#include <map>
#include <opencv2/highgui.hpp>

int main(int argc, char** argv) {
  QApplication app(argc, argv);

  cv::String imgPath =
      QFileDialog::getOpenFileName(nullptr, QObject::tr("Select image"), QDir::homePath(), QObject::tr("Images (*.png *.jpg *.jpeg)"))
          .toStdString();

  if(imgPath.empty()) {
    QMessageBox::critical(nullptr, QObject::tr("Error!"), QObject::tr("Image is not selected!"), QMessageBox::Close);
    return 0;
  }

  QMessageBox::StandardButton answer =
      QMessageBox::question(nullptr, QObject::tr("Debug mode"), QObject::tr("Enable debug mode?"), QMessageBox::Yes | QMessageBox::No);
  switch(answer) {
    case QMessageBox::Yes: {
      misc::ocr::internal::constant::__debugMode = true;
      break;
    }
    default: break;
  }

  misc::ocr::initGlobalAPI();

  cv::utils::logging::setLogLevel(cv::utils::logging::LOG_LEVEL_VERBOSE);

  try {
    plate::number::CarPlateNumberDetector_Russian detector(imgPath);
    if(detector.isValid()) {
      std::list<cv::Mat> plateNumbers = detector.plateNumbers();
      // display only the first image
      cv::String text;
      if(plateNumbers.size() > 0) { text = misc::ocr::extractTextFromImage(plateNumbers.front()); }

      QMessageBox msgBox(QMessageBox::NoIcon,
                         text.size() > 0 ? QObject::tr("Found a russian car plate number!") : QObject::tr("Error!"),
                         text.size() > 0 ? QString::fromStdString(text) : QObject::tr("No found a car plate number!"),
                         text.size() > 0 ? QMessageBox::Ok : QMessageBox::Close);

      QFont cachedFont = msgBox.font();
      msgBox.setFont(QFont("monospace", 24));

      msgBox.button(text.size() > 0 ? QMessageBox::Ok : QMessageBox::Close)->setFont(QFont("monospace", 12));
      msgBox.exec();
    }
  } catch(const cv::Exception& ex) {
    CV_ERROR(ex.msg);
    QMessageBox::critical(nullptr, QObject::tr("Error!"), QString::fromStdString(ex.msg));
  }

  misc::ocr::destroyGlobalAPI();

  return 0;
}
