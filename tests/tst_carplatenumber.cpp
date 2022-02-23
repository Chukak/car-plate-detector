#include "carplatenumberdetector_russian.h"
#include "misc/ocr.h"

#include <gtest/gtest.h>

class CarPlateNumber_Russian_Test : public ::testing::Test {
  public:
  void SetUp() final { misc::ocr::initGlobalAPI(); }
  void TearDown() final { misc::ocr::destroyGlobalAPI(); }
  void SetNumber(const cv::String& n) {
    detector = plate::number::CarPlateNumberDetector_Russian(LoadImage(n));
    number = n;
  }

  private:
  cv::String LoadImage(const cv::String& number) {
    static const cv::String _TestRusCarPlateNumberDir = SOURCE_DIR "/tests/russian/numbers/";

    cv::String fullImageName = "num_" + number + ".png";

    return _TestRusCarPlateNumberDir + fullImageName;
  }

  public:
  plate::number::CarPlateNumberDetector_Russian detector;
  cv::String number;
};

TEST_F(CarPlateNumber_Russian_Test, K574MP77) {
  SetNumber("К574МР77");

  EXPECT_TRUE(detector.isValid());
  EXPECT_EQ(detector.plateNumbers().size(), 1);
  if(!::testing::Test::HasFailure()) { EXPECT_EQ(misc::ocr::extractTextFromImage(detector.plateNumbers().front()), number); }
}

TEST_F(CarPlateNumber_Russian_Test, K494TC75) {
  SetNumber("К494ТС75");

  EXPECT_TRUE(detector.isValid());
  EXPECT_EQ(detector.plateNumbers().size(), 1);
  if(!::testing::Test::HasFailure()) { EXPECT_EQ(misc::ocr::extractTextFromImage(detector.plateNumbers().front()), number); }
}

TEST_F(CarPlateNumber_Russian_Test, Y626YO174) {
  SetNumber("У626УО174");

  EXPECT_TRUE(detector.isValid());
  EXPECT_EQ(detector.plateNumbers().size(), 1);
  if(!::testing::Test::HasFailure()) { EXPECT_EQ(misc::ocr::extractTextFromImage(detector.plateNumbers().front()), number); }
}

TEST_F(CarPlateNumber_Russian_Test, M077MM78) {
  SetNumber("М077ММ78");

  EXPECT_TRUE(detector.isValid());
  EXPECT_EQ(detector.plateNumbers().size(), 1);
  if(!::testing::Test::HasFailure()) { EXPECT_EQ(misc::ocr::extractTextFromImage(detector.plateNumbers().front()), number); }
}

TEST_F(CarPlateNumber_Russian_Test, O950OP72) {
  SetNumber("О950ОР72");

  EXPECT_TRUE(detector.isValid());
  EXPECT_EQ(detector.plateNumbers().size(), 1);
  if(!::testing::Test::HasFailure()) { EXPECT_EQ(misc::ocr::extractTextFromImage(detector.plateNumbers().front()), number); }
}
