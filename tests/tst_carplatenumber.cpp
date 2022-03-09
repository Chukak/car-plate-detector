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

    cv::String fullImageName = number + ".png";

    return _TestRusCarPlateNumberDir + fullImageName;
  }

  public:
  plate::number::CarPlateNumberDetector_Russian detector;
  cv::String number;
};

//! Use it only with the CarPlateNumber_Russian_Test class.
#define CarPlateNumber_Russian_Test_Checks                                                                                      \
  {                                                                                                                             \
    EXPECT_TRUE(detector.isValid());                                                                                            \
    EXPECT_EQ(detector.plateNumbers().size(), 1);                                                                               \
    if(!::testing::Test::HasFailure()) { EXPECT_EQ(misc::ocr::extractTextFromImage(detector.plateNumbers().front()), number); } \
  }

TEST_F(CarPlateNumber_Russian_Test, K574MP77) {
  SetNumber("К574МР77");
  CarPlateNumber_Russian_Test_Checks
}
TEST_F(CarPlateNumber_Russian_Test, K494TC75) {
  SetNumber("К494ТС75");
  CarPlateNumber_Russian_Test_Checks
}
TEST_F(CarPlateNumber_Russian_Test, Y626YO174) {
  SetNumber("У626УО174");
  CarPlateNumber_Russian_Test_Checks
}
TEST_F(CarPlateNumber_Russian_Test, M077MM78) {
  SetNumber("М077ММ78");
  CarPlateNumber_Russian_Test_Checks
}
TEST_F(CarPlateNumber_Russian_Test, O950OP72) {
  SetNumber("О950ОР72");
  CarPlateNumber_Russian_Test_Checks
}
TEST_F(CarPlateNumber_Russian_Test, E794TK35) {
  SetNumber("Е794ТК35");
  CarPlateNumber_Russian_Test_Checks
}
TEST_F(CarPlateNumber_Russian_Test, E496TY35) {
  SetNumber("Е496ТУ35");
  CarPlateNumber_Russian_Test_Checks
}
/* TEST_F(CarPlateNumber_Russian_Test, K407HB73) {
  // TODO: result is К407НВ75, last character is 5 instead of 3
  SetNumber("К407НВ73");
  CarPlateNumber_Russian_Test_Checks
}*/
/*TEST_F(CarPlateNumber_Russian_Test, B494OC21) {
  // TODO: result is В494ОС24, last character is 4 instead of 1
  SetNumber("В494ОС21");
  CarPlateNumber_Russian_Test_Checks
}*/
TEST_F(CarPlateNumber_Russian_Test, B496CH16) {
  SetNumber("В496СН16");
  CarPlateNumber_Russian_Test_Checks
}
TEST_F(CarPlateNumber_Russian_Test, K499MH190) {
  SetNumber("К499МН190");
  CarPlateNumber_Russian_Test_Checks
}
TEST_F(CarPlateNumber_Russian_Test, Y495XT42) {
  SetNumber("У495ХТ42");
  CarPlateNumber_Russian_Test_Checks
}
TEST_F(CarPlateNumber_Russian_Test, M477OC78) {
  SetNumber("М477ОС78");
  CarPlateNumber_Russian_Test_Checks
}
TEST_F(CarPlateNumber_Russian_Test, X894TE190) {
  SetNumber("Х894ТЕ190");
  CarPlateNumber_Russian_Test_Checks
}
/*TEST_F(CarPlateNumber_Russian_Test, M314TH75) {
  // TODO: result is М344ТН75, the second character is 4 instead of 1
  SetNumber("М314ТН75");
  CarPlateNumber_Russian_Test_Checks
}*/
TEST_F(CarPlateNumber_Russian_Test, Y494PY178) {
  SetNumber("У494РУ178");
  CarPlateNumber_Russian_Test_Checks
}
/*TEST_F(CarPlateNumber_Russian_Test, P949OH31) {
  // TODO: result is Р949ОН34, last character is 4 instead of 1
  SetNumber("Р949ОН31");
  CarPlateNumber_Russian_Test_Checks
}*/
TEST_F(CarPlateNumber_Russian_Test, X358BA65) {
  SetNumber("Х358ВА65");
  CarPlateNumber_Russian_Test_Checks
}
TEST_F(CarPlateNumber_Russian_Test, P766PP36) {
  SetNumber("Р766РР36");
  CarPlateNumber_Russian_Test_Checks
}
TEST_F(CarPlateNumber_Russian_Test, K776EM774) {
  SetNumber("К776ЕМ774");
  CarPlateNumber_Russian_Test_Checks
}
TEST_F(CarPlateNumber_Russian_Test, X003BY136) {
  SetNumber("Х003ВУ136");
  CarPlateNumber_Russian_Test_Checks
}
/*TEST_F(CarPlateNumber_Russian_Test, O155AY797) {
  // TODO: result is О455АУ797, the second character is 4 instead of 1
  SetNumber("О155АУ797");
  CarPlateNumber_Russian_Test_Checks
}*/
/*TEST_F(CarPlateNumber_Russian_Test, K612PE58) {
  // TODO: result is К642РЕ58, the second character is 4 instead of 1
  SetNumber("К612РЕ58");
  CarPlateNumber_Russian_Test_Checks
}*/
TEST_F(CarPlateNumber_Russian_Test, A999PT36) {
  SetNumber("А999РТ36");
  CarPlateNumber_Russian_Test_Checks
}
/*TEST_F(CarPlateNumber_Russian_Test, K274ET134) {
  // TODO: result is К474ЕТ134, the second character is 4 instead of 2
  SetNumber("К274ЕТ134");
  CarPlateNumber_Russian_Test_Checks
}*/
TEST_F(CarPlateNumber_Russian_Test, M262TE27) {
  SetNumber("М262ТЕ27");
  CarPlateNumber_Russian_Test_Checks
}
TEST_F(CarPlateNumber_Russian_Test, Y004AT136) {
  SetNumber("У004АТ136");
  CarPlateNumber_Russian_Test_Checks
}
/*TEST_F(CarPlateNumber_Russian_Test, Y976BA71) {
  // TODO: result is У976ВА74, last character is 4 instead of 1
  SetNumber("У976ВА71");
  CarPlateNumber_Russian_Test_Checks
}*/
/*TEST_F(CarPlateNumber_Russian_Test, A111AA69) {
  // TODO: result is А444АА69б, 2-4 characters is 4 instead of 1
  SetNumber("А111АА69");
  CarPlateNumber_Russian_Test_Checks
}*/
/*TEST_F(CarPlateNumber_Russian_Test, A444AA69) {
  // TODO: result is К474ЕТ134, the second character is 2 instead of 4
  SetNumber("А444АА69");
  CarPlateNumber_Russian_Test_Checks
}*/
