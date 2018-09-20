#ifndef ___IMAGE_PROCESSOR___
#define ___IMAGE_PROCESSOR___

#include <string>
#include <opencv2/core.hpp>

class ImageProcessor
{
public:
    ImageProcessor(void);

    void processor(cv::Mat image);

    std::vector<std::string> getData(void);

    ~ImageProcessor(void);
private:
    std::vector<std::string> image_data_;
};

#endif //___IMAGE_PROCESSOR___