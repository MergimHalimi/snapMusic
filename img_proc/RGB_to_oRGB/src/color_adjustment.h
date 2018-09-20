/*****************************************************************
   FILE:      ColorAdjustment.h
   AUTHOR:    Mergim Halimi
   DUE DATE:

   PURPOSE:   The definition of the functions in class ColorAdjustment
****************************************************************/

#ifndef ___COLOR_ADJUSTMENT_H___
#define ___COLOR_ADJUSTMENT_H___

#include	<iostream>

//OpenCV Library
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

//Eigen Library
#include <Eigen/Dense>
#include <Eigen/Geometry>


/**
* ColorAdjustment Class
*/
class ColorAdjustment {

public:

  /**
  * ColorAdjustment constructor
  */
  ColorAdjustment(void);

  enum class Set : uint8_t 
  {
    L = 0,
    Cyb = 1,
    Crg = 2
  };

  /**
  * ColorAdjustment constructor with param
  */
  ColorAdjustment(cv::Mat image_);

  /**
  * ColorAdjustment constructor with param
  */
  cv::Mat adjustColor(Set deviate_, double alpha_ = 1, double beta_ = 0);

  std::pair<cv::Scalar, cv::Scalar> calculateAlpha(cv::Mat image_);

  /**
  * ColorAdjustment destructor
  */
  ~ColorAdjustment();

private:

  cv::Mat image_;

};
#endif