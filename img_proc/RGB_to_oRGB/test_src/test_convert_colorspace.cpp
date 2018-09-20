/****************************************************************
   FILE:      test_trajectory_mover.cpp
   AUTHOR:    Mergim Halimi
   DUE DATE:  03 Apr 2018
****************************************************************/

#include "test_convert_colorspace.h"

extern std::string tests_data_path_;

/*
* @brief 
* @brief 
*/
TEST_F(TestConvertColorspace, loadFromFileRightPathExpectTrue) {
  ConvertColorspace image_;

  EXPECT_TRUE(image_.loadFromfile( tests_data_path_ + "/image.png"));
}

/*
* @brief
* @brief
*/
TEST_F(TestConvertColorspace, checkImagePixels) {

  //Initialise a class
  ConvertColorspace image_;

  image_.loadFromfile( tests_data_path_ + "/image.png");

  // Original image pixel (Readen from Matlab) = [27, 146, 100]
  // Expect here the same when I access the pixel
  EXPECT_NEAR(27, image_.accessImgPixel(image_.OriginalImage("CV_64FC3"), 0, 0)[2], 0.0001);
  EXPECT_NEAR(146, image_.accessImgPixel(image_.OriginalImage("CV_64FC3"), 0, 0)[1], 0.0001);
  EXPECT_NEAR(100, image_.accessImgPixel(image_.OriginalImage("CV_64FC3"), 0, 0)[0], 0.0001);

  // LCC image pixel (Calculated from Matlab) = [167.6730, 2.2490, -142.9220]
  // Expect here the same when I access the pixel
  EXPECT_NEAR(167.6730, image_.accessImgPixel(image_.LCCImage("CV_64FC3"), 0, 0)[2], 0.0001);
  EXPECT_NEAR(2.2490, image_.accessImgPixel(image_.LCCImage("CV_64FC3"), 0, 0)[1], 0.0001);
  EXPECT_NEAR(-142.9220, image_.accessImgPixel(image_.LCCImage("CV_64FC3"), 0, 0)[0], 0.0001);

  // RGB image pixel (Converted from LCC image with the inverse transformation matrix)
  // Expect here the same pixel as in the original image
  EXPECT_NEAR(image_.accessImgPixel(image_.OriginalImage("CV_64FC3"), 0, 0)[0], image_.accessImgPixel(image_.RGBImage("CV_64FC3"), 0, 0)[0], 0.0001);
  EXPECT_NEAR(image_.accessImgPixel(image_.OriginalImage("CV_64FC3"), 0, 0)[1], image_.accessImgPixel(image_.RGBImage("CV_64FC3"), 0, 0)[1], 0.0001);
  EXPECT_NEAR(image_.accessImgPixel(image_.OriginalImage("CV_64FC3"), 0, 0)[2], image_.accessImgPixel(image_.RGBImage("CV_64FC3"), 0, 0)[2], 0.0001);

  // oRGB image pixel (Calculated from Matlab) = [167.6730, 101.87, -100.28]
  // Expect here the same when I access the pixel
  EXPECT_NEAR(167.6730, image_.accessImgPixel(image_.oRGBImage("CV_64FC3"), 0, 0)[2], 0.0001);
  EXPECT_NEAR(101.87, image_.accessImgPixel(image_.oRGBImage("CV_64FC3"), 0, 0)[1], 0.01);
  EXPECT_NEAR(-100.28, image_.accessImgPixel(image_.oRGBImage("CV_64FC3"), 0, 0)[0], 0.01);

  // LCC image pixel (Converted from oRGB image with the inverse rotation matrix)
  // Expect here the same pixel as in the original image
  std::cout << image_.accessImgPixel(image_.LCCImage("CV_64FC3"), 0, 0) << std::endl
            << image_.accessImgPixel(image_.LCCImageConverted("CV_64FC3"), 0, 0) << std::endl;

  EXPECT_NEAR(image_.accessImgPixel(image_.LCCImage("CV_64FC3"), 0, 0)[0], image_.accessImgPixel(image_.LCCImageConverted("CV_64FC3"), 0, 0)[0], 0.0001);
  EXPECT_NEAR(image_.accessImgPixel(image_.LCCImage("CV_64FC3"), 0, 0)[1], image_.accessImgPixel(image_.LCCImageConverted("CV_64FC3"), 0, 0)[1], 0.0001);
  EXPECT_NEAR(image_.accessImgPixel(image_.LCCImage("CV_64FC3"), 0, 0)[2], image_.accessImgPixel(image_.LCCImageConverted("CV_64FC3"), 0, 0)[2], 0.0001);

}