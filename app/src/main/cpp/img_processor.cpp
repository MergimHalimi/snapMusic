#include "img_processor.h"

ImageProcessor::ImageProcessor(void)
{
  //
}

bool ImageProcessor::initialize(cv::Mat image)
{
  if (image.empty())
  {
    this->is_initialized_ = false;
    return false;
  }
  else
  {
    this->image_ = image;
    this->is_initialized_ = true;
    return true;
  }
}

bool ImageProcessor::runProccessor(void)
{
  if (!this->is_initialized_)
  {
    this->is_running_ = false;
    return false;
  }
  else
  {
    //run the processor
    processor(this->image_);
    this->is_running_ = true;
    return true;
  }
}

bool ImageProcessor::isValid(void)
{
  bool is_valid_ = true;
  is_valid_ &= this->is_initialized_;
  is_valid_ &= this->is_running_;

  return is_valid_;
}

std::vector<std::string> ImageProcessor::getData(void)
{

  if (!isValid())
  {
    this->image_data_.clear();
    this->image_data_.push_back("Not initialized, or the processor not running");
  }
  else
  {
    this->image_data_.clear();
    this->image_data_.push_back("Chair");
    this->image_data_.push_back("Car");
    this->image_data_.push_back("Test_1");
    this->image_data_.push_back("Test_2");
    this->image_data_.push_back("Test_3");
  }

  return this->image_data_;
}

ImageProcessor::~ImageProcessor(void)
{
  //
}
///////////////////

void ImageProcessor::processor(cv::Mat image)
{
  //
}