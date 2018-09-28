#ifndef ___IMAGE_PROCESSOR___
#define ___IMAGE_PROCESSOR___

//C++
#include <fstream>
#include <string>
#include <vector>

//OpenCV
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/dnn.hpp>

/**
 * \brief This class holds the functionality to get the image, process it, and return a vector of strings as a result of the objects recognised
 *        from the class. The algorithms used is YOLOV3 (You Only Look Once) and uses a pre trained weights and classes model.
 */
class ImageProcessor
{
public:
  /**
   * \brief ImageProcessor Class Constructor
   */
  ImageProcessor(void);

  //Avoid copy
  ImageProcessor(const ImageProcessor&) = delete;
  //Allow move
  ImageProcessor(ImageProcessor&&) = default;

  /**
   * \brief ImageProcessor Class initializer
   * \param image : cv::Mat - Image
   * \return bool - True or false
   */
  bool initialize(cv::Mat image);

  /**
   * \brief ImageProcessor Class initializer
   * \param image : cv::Mat - Image
   * \return bool - True or false
   */
  bool runProcessor(void);

  /**
   * \brief ImageProcessor Class initializer
   * \param image : cv::Mat - Image
   * \return bool - True or false
   */
  bool isValid(void);

  /**
   * \brief ImageProcessor Class result getter method.
   * \return std::vector<std::string> - True or false
   */
  std::vector<std::string> getData(void);

  /**
   * \brief ImageProcessor Class Destructor
   */
  ~ImageProcessor(void);

private:
  /**
   * \brief ImageProcessor Class Destructor
   */
  void processor(cv::Mat& image);

  /**
   * \brief ImageProcessor Class function initYOLOParams, to initialize the parameters for the image processor
   */
  bool initYOLOParams(void);

  /**
  * \brief ImageProcessor Class function loadYOLOModel, to load the pretrained weights and model, and classes for image recognition.
  */
  bool loadYOLOModel(void);

  /**
  * \brief ImageProcessor Class function loadImage, to load the image and prepare for the postprocess (Final process of the recognition)
  */
  bool loadImage(cv::Mat& image);

  /**
  * \brief ImageProcessor Class function filterWithConfidence, the final process of image recognition and result writing (Remove results with low confidence)
  */
  bool filterWithConfidence(cv::Mat detections);

  /**
   * \param image_ : cv::Mat - The variable to store the input image
   * \param image_data_ : std::vector<std::string> - The variable to store the output result as vector of strings
   * \param is_initialized_ : bool -
   * \param is_running_ : bool -
   *
   * \param conf_threshold_ : float - Confidence threshold
   * \param nms_threshold_ : float - Non-maximum suppression threshold
   * \param input_width_ : int - Width of network's input image
   * \param input_height_ : int - Height of network's input image
   * \param model_configuration_ : std::string -
   * \param model_weights_ : std::string -
   * \param classes_ : std::vector<std::string> -
   * \param net_ : cv::dnn::Net -
   * \param detections_ : cv::Mat -
   * \param input_height_ : int -
   * \param input_height_ : int -
   * \param input_height_ : int -
   * \param input_height_ : int -
   * \param input_height_ : int -
   */
  cv::Mat image_;
  std::vector<std::string> image_data_;
  bool is_initialized_ = false,
       is_running_ = false;

  float conf_threshold_,
        nms_threshold_;
  int input_width_,
      input_height_;
  std::string classes_file_;
  cv::String model_configuration_,
             model_weights_;
  std::vector<std::string> classes_;
  cv::dnn::Net net_;
  cv::Mat detections_;
};
#endif //___IMAGE_PROCESSOR___