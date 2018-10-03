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

bool ImageProcessor::runProcessor(void)
{
  if (!this->is_initialized_)
  {
    this->is_running_ = false;
    return false;
  }
  else
  {
    //run the processor
    this->processor(this->image_);TODO:
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

  if (!this->isValid())
  {
    this->image_data_.clear();
    this->image_data_.push_back("Not initialized, the processor not running or\nPermisions not allowed");
  }
  else
  {
    //
  }

  return this->image_data_;
}

ImageProcessor::~ImageProcessor(void)
{
  //
}
///////////////////

void ImageProcessor::processor(cv::Mat& image)
{
  this->initYOLOParams();

  this->loadYOLOModel();

  this->loadImage(image);

  this->filterWithConfidence(this->detections_);
}

bool ImageProcessor::initYOLOParams(void)
{
  this->conf_threshold_ = 0.24;
  this->nms_threshold_ = 0.4;
  this->input_width_ = 416;
  this->input_height_ = 416;

  return true;
}

bool ImageProcessor::loadYOLOModel(void)
{
  // Load names of classes
  this->classes_file_ = "/storage/emulated/0/datasets/coco.names";
  std::ifstream ifs(classes_file_.c_str());
  std::string line;

  while (std::getline(ifs, line))
  {
    classes_.push_back(line);
  }

  //Give the configuration and weight files for the model
  this->model_configuration_ = "/storage/emulated/0/datasets/yolov3.cfg";
  this->model_weights_ = "/storage/emulated/0/datasets/yolov3.weights";

  //Load the network
  this->net_ = cv::dnn::readNetFromDarknet(this->model_configuration_, this->model_weights_);
  this->net_.setPreferableBackend(cv::dnn::DNN_BACKEND_OPENCV);
  this->net_.setPreferableTarget(cv::dnn::DNN_TARGET_CPU);

  return true;
}

bool ImageProcessor::loadImage(cv::Mat& image)
{
  cv::Mat blob_;

  // Create a 4D blob from a frame.
  cv::dnn::blobFromImage(image, blob_, 1/255.0, cv::Size(this->input_width_, this->input_height_), cv::Scalar(0,0,0), true, false);

  //Sets the input to the network
  this->net_.setInput(blob_);

  this->detections_ = this->net_.forward();

  return true;
}

bool ImageProcessor::filterWithConfidence(cv::Mat detections)
{
  this->image_data_.clear();

  for (int i = 0; i < detections.rows; i++)
  {
    const int probability_index_ = 5;
    const int probability_size_ = detections.cols - probability_index_;

    float *prob_array_ptr_ = &detections.at<float>(i, probability_index_);

    size_t objectClass = std::max_element(prob_array_ptr_, prob_array_ptr_ + probability_size_) - prob_array_ptr_;

    float confidence_ = detections.at<float>(i, static_cast<int>(objectClass) + probability_index_);

    if (confidence_ >= this->conf_threshold_)
    {
      std::string class_name_ = objectClass < classes_.size() ?
                              classes_[objectClass] :
                              "CLASSES::UNCLASSIFIED";

      std::string filtered_result_ = "Class: " + class_name_ + ", Confidence: " + std::to_string(confidence_);

      this->image_data_.push_back(filtered_result_);
    }
  }

  return true;
}