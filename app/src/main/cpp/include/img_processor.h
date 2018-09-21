#ifndef ___IMAGE_PROCESSOR___
#define ___IMAGE_PROCESSOR___

#include <string>
#include <opencv2/core.hpp>

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
    bool runProccessor(void);

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
    void processor(cv::Mat image);

    /**
     * \param image_ : cv::Mat -
     * \param image_data_ : std::vector<std::string> -
     * \param image_ : cv::Mat -
     * \param image_ : cv::Mat -
     * \param image_ : cv::Mat -
     * \param image_ : cv::Mat -
     * \param image_ : cv::Mat -
     */
    cv::Mat image_;
    std::vector<std::string> image_data_;
    bool is_initialized_ = false;
    bool is_running_ = false;
};

#endif //___IMAGE_PROCESSOR___