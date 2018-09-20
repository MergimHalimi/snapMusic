/****************************************************************
   PROGRAM:    RGB to oRGB
   AUTHOR:     Mergim Halimi   m.halimi@live.com
   DUE DATE:   

   FUNCTION:   This app is used to convert colorspace from RGB
               to oRGB and apply a warm to cool filter

   OUTPUT:     Output will be an image that contains nine images 
               inside, with the original one in middle, and 
               converted images around

   NOTES:      N/A
****************************************************************/

#include "convert_colorspace.h"
#include "color_adjustment.h"

#include <iostream>
#include <stdio.h>
#include <stdarg.h>

//Eigen Library
#include <Eigen/Dense>
#include <Eigen/Geometry>

int main(int argc, char** argv) {
  //Create class instance
  ConvertColorspace image_;

  // Load image,
  image_.loadFromfile ("/home/jimmyhalimi/Desktop/rgb_to_orgb/Test_Files/image.png");

  //Set the orgb image to object
  ColorAdjustment oRGB_image_deviated_ =  ColorAdjustment(image_.oRGBImage());

  // Create 1280x480 mat for window
  cv::Mat result_(cv::Size(800, 1133), CV_8UC3);

  // Copy small images into a big mat, imagine a 3x3 Matrix
  //Image (0,0)
  image_.LCCImage().copyTo(result_(cv::Rect(  0  , 0, 260, 371)));
  //Image (1,0)
  oRGB_image_deviated_.adjustColor(ColorAdjustment::Set::Crg, 2, 0).copyTo(result_(cv::Rect(  270, 0, 260, 371)));
  //Image (2,0)
  image_.oRGBImage().copyTo(result_(cv::Rect(  540, 0, 260, 371)));
  //Image (0,1)
  oRGB_image_deviated_.adjustColor(ColorAdjustment::Set::Cyb, 0.5, 0).copyTo(result_(cv::Rect(  0  , 381, 260, 371)));
  //Image (1,1) - Original Image
  image_.OriginalImage().copyTo(result_(cv::Rect(  270, 381, 260, 371)));
  //Image (2,1)
  oRGB_image_deviated_.adjustColor(ColorAdjustment::Set::Cyb, 2, 0).copyTo(result_(cv::Rect(  540, 381, 260, 371)));
  //Image (0,2)
  //oRGB_image_deviated_.adjustColor(ColorAdjustment::Set::R, 1, 0).copyTo(result_(cv::Rect(  0  , 762, 260, 371)));
  //Image (1,2)
  oRGB_image_deviated_.adjustColor(ColorAdjustment::Set::Crg, 0.5, 0).copyTo(result_(cv::Rect(  270, 762, 260, 371)));
  //Image (2,2)
  //oRGB_image_deviated_.adjustColor(ColorAdjustment::Set::R, 1, 0).copyTo(result_(cv::Rect(  540, 762, 260, 371)));
    
  //printf("Screen resolution: %dx%d\n", width, height);
 
  //Write image to disk
  //cv::imwrite("/home/jimmyhalimi/Desktop/rgb_to_orgb/Test_Files/Output/Results.png", result_);

  // It displays three windows representing three channels
  image_.splitChannels(image_.oRGBImage());

  // Display big mat
  cv::namedWindow( "Result", CV_WINDOW_FULLSCREEN);
  cv::imshow("Result", result_);

  std::cout << "Press any key to close ..." << std::endl;
  cv::waitKey(0);
  cv::destroyAllWindows();

  return 0;
}