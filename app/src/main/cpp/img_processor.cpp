#include "img_processor.h"

/**
 * \brief ImageProcessor Class Constructor
 */
ImageProcessor::ImageProcessor(void)
{
  //
}

/**
 * \brief ImageProcessor Class Constructor
 */
void ImageProcessor::processor(cv::Mat image)
{
  //
}

/**
 * \brief ImageProcessor Class Constructor
 */
std::vector<std::string> ImageProcessor::getData(void)
{
  this->image_data_.push_back("TEST");
  this->image_data_.push_back("LESH");
  this->image_data_.push_back("2");
  this->image_data_.push_back("3");
  this->image_data_.push_back("4");

  return this->image_data_;
}

/**
 * \brief ImageProcessor Class Destructor
 */
ImageProcessor::~ImageProcessor(void)
{
  //
}