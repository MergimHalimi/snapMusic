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
  this->conf_threshold_ = 0.10;
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

      //Data vector
      this->image_data_.push_back(filtered_result_);

      //Image with draw results
      float x_center = detections.at<float>(i, 0) * this->image_.cols;
      float y_center = detections.at<float>(i, 1) * this->image_.rows;
      float width = detections.at<float>(i, 2) * this->image_.cols;
      float height = detections.at<float>(i, 3) * this->image_.rows;

      cv::Point p1(cvRound(x_center - width / 2), cvRound(y_center - height / 2));
      cv::Point p2(cvRound(x_center + width / 2), cvRound(y_center + height / 2));
      cv::Rect object(p1, p2);

      //Box color
      cv::Scalar object_roi_color;
      if (confidence_ >= 0 && confidence_ < 0.30)
      {
        object_roi_color = cv::Scalar(255, 0, 0);
      }
      else if (confidence_ >= 0.30 && confidence_ < 0.60)
      {
        object_roi_color = cv::Scalar(0, 0, 255);
      }
      else if (confidence_ >= 0.60 && confidence_ <= 1.0)
      {
        object_roi_color = cv::Scalar(0, 255, 0);
      }

      rectangle(this->image_, object, object_roi_color);

      cv::String label = cv::format("%s: %.2f", class_name_.c_str(), confidence_);
      int baseLine = 0;
      cv::Size labelSize = getTextSize(label, cv::FONT_HERSHEY_SIMPLEX, 0.5, 1, &baseLine);

      rectangle(this->image_, cv::Rect(p1, cv::Size(labelSize.width, labelSize.height + baseLine)),
                object_roi_color, cv::FILLED);

      putText(this->image_, label, p1 + cv::Point(0, labelSize.height),
              cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(0,0,0));
    }
  }

  return true;
}