/*****************************************************************
   FILE:      ConvertColorspace.h
   AUTHOR:    Mergim Halimi
   DUE DATE:

   PURPOSE:   The definition of the functions in class ConvertColorspace
****************************************************************/

#ifndef ___CONVERT_COLORSPACE_H___
#define ___CONVERT_COLORSPACE_H___

#include	<iostream>

//OpenCV Library
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

//Eigen Library
#include <Eigen/Dense>
#include <Eigen/Geometry>

//
#include <math.h>

/**
* ConvertColorspace Class
*/
class ConvertColorspace {

public:

  /**
  * ConvertColorspace constructor
  */
  ConvertColorspace(void);

  /**
  * @brief - This function loads the image and stores it in a private class member
  * @param path - The input argument is a string of the filepath
  * @return ConvertColorspace::loadFromfile - Returns false if the file is not loaded properly
  */
  bool loadFromfile(const std::string path);

  /**
  * @brief - This function shows three channels of the image parset as arg
  * @param image_ - The input argument is a cv::Mat
  */
	void splitChannels(cv::Mat image_);

  /**
  * @brief - This function returns the original image storen in the Private: of the class
  * @usage - OBJECT.OriginalImage();
  * @return cv::Mat - Original image in cv::Mat format
  */
	const inline cv::Mat OriginalImage(std::string temp_ = "CV_8UC3") const noexcept
	{
    if (temp_ == "CV_8UC3")
    {
      cv::Mat m;
      m = this->original_image_.clone();
      //cvtColor(m, m, CV_RGB2BGR);
	  	return m;
    }
    else if (temp_ == "CV_64FC3")
    {
      cv::Mat m;
      m = this->original_image_.clone();
      //cvtColor(m, m, CV_RGB2BGR);
      m.convertTo(m, CV_64FC3);
      return m;
    }
	}

  /**
  * @brief - This function returns the original image storen in the Private: of the class
  * @usage - OBJECT.OriginalImage();
  * @return cv::Mat - Original image in cv::Mat format
  */
	const inline cv::Mat OriginalImageCorrected(std::string temp_ = "CV_8UC3") const noexcept
	{
    if (temp_ == "CV_8UC3")
    {
      cv::Mat m;
      m = this->image_.clone();
      cvtColor(m, m, CV_RGB2BGR);
	  	return m;
    }
    else if (temp_ == "CV_64FC3")
    {
      cv::Mat m;
      m = this->image_.clone();
      //cvtColor(m, m, CV_RGB2BGR);
      m.convertTo(m, CV_64FC3);
      return m;
    }
	}

  /**
  * @brief - This function returns the image in LCC colorspace, storen in the Private: of the class
  * @usage - OBJECT.LCCImage();
  * @return cv::Mat - Original image in cv::Mat format
  */
	inline cv::Mat LCCImage(std::string temp_ = "CV_8UC3") const noexcept
	{
    if (temp_ == "CV_8UC3")
    {
      cv::Mat m;
      m = this->LCC_image_.clone();
      //this->LCC_image_.convertTo(m, CV_8UC3);
      cvtColor(m, m, CV_RGB2BGR);
      //cvtColor(m, m, CV_RGB2XYZ);
      return m;
    }
    else if (temp_ == "CV_64FC3")
    {
      cv::Mat m;
      m = this->LCC_image_.clone();
      //cvtColor(m, m, CV_RGB2BGR);
      m.convertTo(m, CV_64FC3);
      return m;
    }
	}

  /**
  * @brief - This function returns the image in LCC colorspace, storen in the Private: of the class
  * @usage - OBJECT.LCCImage();
  * @return cv::Mat - Original image in cv::Mat format
  */
	inline cv::Mat LCCImageConverted(std::string temp_ = "CV_8UC3") const noexcept
	{
    if (temp_ == "CV_8UC3")
    {
      cv::Mat m;
      m = this->LCC_image_converted_.clone();
      //this->LCC_image_converted_.convertTo(m, CV_8UC3);
      cvtColor(m, m, CV_RGB2BGR);
      return m;
    }
    else if (temp_ == "CV_64FC3")
    {
      cv::Mat m;
      m = this->LCC_image_converted_.clone();
      //cvtColor(m, m, CV_RGB2BGR);
      m.convertTo(m, CV_64FC3);
      return m;
    }
	}

  /**
  * @brief - This function returns the image in RGB colorspace, storen in the Private: of the class
  * @brief - and converted back from LCC colorspace
  * @usage - OBJECT.RGBImage();
  * @return cv::Mat - Original image in cv::Mat format
  */
	inline cv::Mat RGBImage(std::string temp_ = "CV_8UC3") const noexcept
	{
    if (temp_ == "CV_8UC3")
    {
      cv::Mat m;
      m = this->RGB_image_.clone();
      //this->RGB_image_.convertTo(m, CV_8UC3);
      cvtColor(m, m, CV_RGB2BGR);
      return m;
    }
    else if (temp_ == "CV_64FC3")
    {
      cv::Mat m;
      m = this->RGB_image_.clone();
      //cvtColor(m, m, CV_RGB2BGR);
      m.convertTo(m, CV_64FC3);
      return m;
    }
	}

  /**
  * @brief - This function returns the image in oRGB colorspace, storen in the Private: of the class
  * @brief - converted from LCC colorspace
  * @usage - OBJECT.oRGBImage();
  * @return cv::Mat - Original image in cv::Mat format
  */
	inline cv::Mat oRGBImage(std::string temp_ = "CV_8UC3") const noexcept
	{
    if (temp_ == "CV_8UC3")
    {
      cv::Mat m;
      m = this->oRGB_image_.clone();
      //this->oRGB_image_.convertTo(m, CV_8UC3);
      cvtColor(m, m, CV_RGB2BGR);
      //cvtColor(m, m, cv::COLOR_RGB2YCrCb);
      return m;
    }
    else if (temp_ == "CV_64FC3")
    {
      cv::Mat m;
      m = this->oRGB_image_.clone();
      //cvtColor(m, m, CV_RGB2BGR);
      m.convertTo(m, CV_64FC3);
      return m;
    }
	}

  const cv::Vec3b accessImgPixel(const cv::Mat image_, const std::size_t index_row_, const std::size_t index_col_) const noexcept;

  /**
  * ConvertColorspace destructor
  */
  ~ConvertColorspace();

private:

  /**
  * @param[image_] - This is the variable that stores the original image
  * @param[RGB_image_] - This is the variable that stores the RGB_image_ image
  * 
  * @param[LCC_image_] - This is the variable that stores the LCC_image_ image
  * @param[oRGB_image_] - This is the variable that stores the oRGB_image_ image
  */
  cv::Mat image_,
          original_image_,
          RGB_image_, 
          LCC_image_,
          LCC_image_converted_,
          oRGB_image_;

  /**
  * @brief - This function converts an image in RGB Colorspace to L'C'C' colorspace
  * @brief - It is called in the constructor when the image is loaded and storen in private member
  * @brief - Then the function uses that, to procced with the calculations
  * @usage - RGBtoLCC();
  */
	void RGBtoLCC(void);

  /**
  * @brief - This function converts an image in L'C'C' Colorspace to RGB colorspace
  * @brief - It is called in the constructor when the image is loaded, converted and storen in private member
  * @brief - This is the inverse of the RGB to L'C'C'
  * @usage - RGBtoLCC();
  */
	void LCCtoRGB(void);

  /**
  * @brief - This function converts an image in L'C'C' Colorspace to oRRGB colorspace
  * @brief - It is called in the constructor when the image is loaded, converted and storen in private member
  * @brief - Then the function uses that, to procced with the calculations
  * @usage - RGBtoLCC();
  */
  void LCCtooRGB(void);

  /**
  * @brief - This function converts an image in oRGB Colorspace to L'C'C' colorspace by applying the inverse rotation matrix
  * @brief - It is called in the constructor when the image is loaded, converted and storen in private member
  * @brief - Then the function uses that, to procced with the calculations
  * @usage - oRGBtoLCC();
  */
  void oRGBtoLCC(void);

  void GammaCorrection(cv::Mat& image_, double scalar_);

};
#endif