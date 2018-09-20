/****************************************************************
   FILE:      color_adjustment.cpp
   AUTHOR:    Mergim Halimi
   DUE DATE:

   PURPOSE:   The implementations of the functions in class
              ColorAdjustment
****************************************************************/

//Class
#include "color_adjustment.h"

ColorAdjustment::ColorAdjustment()
{
  /*Empty constructor.*/
}

ColorAdjustment::ColorAdjustment(cv::Mat image_)
{
  this->image_ = image_;
}

cv::Mat ColorAdjustment::adjustColor(Set deviate_, double alpha_, double beta_)
{
  cv::Mat deviated_image_ = this->image_.clone();
//TODO check if image empty

  //First mean_, second standard deviation
  std::pair<cv::Scalar, cv::Scalar> calculated_alpha_ = std::make_pair(cv::Scalar(1,1,1), cv::Scalar(1,1,1));
  //std::pair<cv::Scalar, cv::Scalar> calculated_alpha_ = calculateAlpha(deviated_image_);

  cv::MatIterator_<cv::Vec3b> it, end;
  for (it = deviated_image_.begin<cv::Vec3b>(), end = deviated_image_.end<cv::Vec3b>(); it != end; ++it) 
  {
    uchar &r = (*it)[2];
    uchar &g = (*it)[1];
    uchar &b = (*it)[0];

    switch(deviate_)
    {
      case Set::L :
        r = cv::saturate_cast<uchar>((alpha_ * calculated_alpha_.second(2) * r) + beta_);
        // std::cout << "R\n";
        break;
      case Set::Cyb :
        g = cv::saturate_cast<uchar>((alpha_ * calculated_alpha_.second(1) * g) + beta_);
        // std::cout << "G\n";
        break;
      case Set::Crg :
        b = cv::saturate_cast<uchar>((alpha_ * calculated_alpha_.second(0) * b) + beta_);
        // std::cout << "B\n";
        break;
      default :
        // std::cout << "Def\n";
        break;
    }
  }

  return deviated_image_;
}

std::pair<cv::Scalar, cv::Scalar> ColorAdjustment::calculateAlpha(cv::Mat image_)
{
  cv::Scalar mean_, std_deviation_;
  cv::meanStdDev(image_, mean_, std_deviation_);

  cv::MatIterator_<cv::Vec3b> it, end;
  for (it = image_.begin<cv::Vec3b>(), end = image_.end<cv::Vec3b>(); it != end; ++it) 
  {
    uchar &r = (*it)[2];
    uchar &g = (*it)[1];
    uchar &b = (*it)[0];
  }

  auto calculated_alpha_ = std::make_pair(mean_, std_deviation_);

  return calculated_alpha_;
}

ColorAdjustment::~ColorAdjustment() {}