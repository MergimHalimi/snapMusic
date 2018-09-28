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
    //this->image_data_.clear();
    //this->image_data_.push_back("Chair");
    //this->image_data_.push_back("Car");
    //this->image_data_.push_back("Laptop");

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

  //Process frame

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
  cv::Mat blob;
  // Create a 4D blob from a frame.
  blob = cv::dnn::blobFromImage(image, 1/255.0, cv::Size(this->input_width_, this->input_height_), cv::Scalar(0,0,0), true, false);

  //Sets the input to the network
  this->net_.setInput(blob);

  this->detections = this->net_.forward();

  this->filterWithConfidence();

  return true;
}

bool ImageProcessor::filterWithConfidence()
{
  this->image_data_.clear();

  for (int i = 0; i < this->detections.rows; i++)
  {
    const int probability_index = 5;
    const int probability_size = this->detections.cols - probability_index;
    double *prob_array_ptr = &this->detections.at<double>(i, probability_index);

    size_t objectClass = std::max_element(prob_array_ptr, prob_array_ptr + probability_size) - prob_array_ptr;

    float confidence = this->detections.at<float>(i, (int) objectClass + probability_index);

    if (confidence >= this->conf_threshold_)
    {
      std::string className = objectClass < classes_.size() ?
                              classes_[objectClass] :
                              "CLASSES::UNCLASSIFIED";


      std::string _buff = "Class: " + className + ", Confidence: " + std::to_string(confidence);

      this->image_data_.push_back(_buff);
    }
  }

  return true;
}

std::vector<cv::String> ImageProcessor::writeOutputsNames(const cv::dnn::Net& net)
{
  static std::vector<cv::String> names_;

  this->image_data_.clear();
  names_.clear();

  this->image_data_.push_back("From img_processor : ");

  //Get the indices of the output layers, i.e. the layers with unconnected outputs
  std::vector<int> outLayers = net.getUnconnectedOutLayers();

  //get the names of all the layers in the network
  std::vector<cv::String> layers_names_ = net.getLayerNames();

  // Get the names of the output layers in names
  names_.resize(outLayers.size());

  for (size_t i = 0; i < outLayers.size(); ++i)
  {
    this->image_data_.push_back(layers_names_[outLayers[i] - 1]);
    names_[i] = layers_names_[outLayers[i] - 1];
  }

  if (names_.empty())
  {
    this->image_data_.push_back("No objects detected");
  }

  return names_;
}