/****************************************************************
   FILE:      color_adjustment.cpp
   AUTHOR:    Mergim Halimi
   DUE DATE:

   PURPOSE:   The implementations of the functions in class
              ConvertColorspace
****************************************************************/

//Class
#include "convert_colorspace.h"

ConvertColorspace::ConvertColorspace()
{
  /*Empty constructor. Check loadFromfile, using that to load and perform other actions*/
}

bool ConvertColorspace::loadFromfile(const std::string path)
{
  this->original_image_ = cv::imread(path, CV_LOAD_IMAGE_COLOR);
  const int channels = this->original_image_.channels();

  //Check if image is empty or is not in three channel colorspace
  if ( !this->original_image_.data || (channels != 3))
  {
      std::cout << "Image not loaded. Check path : " << path << std::endl
                << "or image not in 3Channels. Channels : " << channels << std::endl;
      return false;
  }

  cvtColor(this->original_image_, this->image_, CV_BGR2RGB);

  GammaCorrection(this->image_, 1);

  /**
  * @brief - Call the convertion functions. (Note: Order matters)
  * @brief - First step : Convert original image into L'C'C Colorspace (This applies the transformation)
  * @brief - Second step : Convert image that is in L'C'C Colorspace into oRGB Colorspace (This applies the rotation)
  * @brief - Third step : Do the Inverse calculation of RGBtoLCC(), the image must be the same as original
  */
  this->RGBtoLCC();
  this->LCCtooRGB();
  this->LCCtoRGB();
  this->oRGBtoLCC();

	return true;
}

void ConvertColorspace::RGBtoLCC(void)
{
  //Create a temp Mat, clone the original image
  cv::Mat _temp = this->image_.clone();
  
  //Convert the temp to 64Bit Floating Point, 3Channels
  //_temp.convertTo(_temp, CV_64FC3);

	Eigen::Matrix3d transformation_matrix_;

	transformation_matrix_ << 0.2990,  0.5870,  0.1140,
                            0.5000,  0.5000, -1.0000,
                            0.8660, -0.8660,  0.0000;

  cv::MatIterator_<cv::Vec3b> it, end;
  for (it = _temp.begin<cv::Vec3b>(), end = _temp.end<cv::Vec3b>(); it != end; ++it) 
  {
      uchar &r = (*it)[0];
      uchar &g = (*it)[1];
      uchar &b = (*it)[2];
      cv::Vec3b &pixel_ = (*it);
      
      Eigen::Vector3d image_rgb_vec_;
      image_rgb_vec_ << r, g, b;

      // Multiply with transformation_matrix_
      Eigen::Vector3d image_rgb_vec_transformed_ = transformation_matrix_.transpose() * image_rgb_vec_;

      r = cv::saturate_cast<uchar>(image_rgb_vec_transformed_(0));
      g = cv::saturate_cast<uchar>(image_rgb_vec_transformed_(1));
      b = cv::saturate_cast<uchar>(image_rgb_vec_transformed_(2));

      // std::cout << "Readen pixel : [" << image_rgb_vec_(0) << ", " 
      //                        << image_rgb_vec_(1) << ", " 
      //                        << image_rgb_vec_(2) << "]" 
      //                        << std::endl;
      // std::cout << "Transformmed pixel vector : [" << image_rgb_vec_transformed_(0) << ", " 
      //                        << image_rgb_vec_transformed_(1) << ", " 
      //                        << image_rgb_vec_transformed_(2) << "]" 
      //                        << std::endl;
                             
      // std::cout << "Setted pixel : " << pixel_ << std::endl;
      
      // std::cout << "Press any key for the next iteration" << std::endl;
      // std::cin.get();
  }

  //Convert the temp to 8Bit Unsigned char 3Channel, into the private Mat
  //Note: this is because imshow cannot display 32 or 64 bit floating point images
  this->LCC_image_ = _temp.clone();
  //_temp.convertTo(this->LCC_image_, CV_8UC3);
}

void ConvertColorspace::LCCtoRGB(void)
{
  //Create a temp Mat, clone the original image
  cv::Mat _temp = this->LCC_image_.clone();
  
  //Convert the temp to 64Bit Floating Point, 3Channels
  //_temp.convertTo(_temp, CV_64FC3);

	Eigen::Matrix3d transformation_matrix_;

  //Here, recalculated the inverse matrix, for better results
  //The matrix was :
  //	transformation_matrix_ << 1.0000,  0.1140,  0.7436,
  //                            1.0000,  0.1140, -0.4111,
  //                            1.0000, -0.8860,  0.1663;
  //
  //  Readen pixel              : [167.673, 2.249, -142.922]
  //  Transformmed pixel vector : [27, 146, 99.9892]
  //  Expected results          : [27, 146, 100]
	transformation_matrix_ << 1.0000,  0.1140,  0.743649,
                            1.0000,  0.1140, -0.411055,
                            1.0000, -0.8860,  0.166282;

  cv::MatIterator_<cv::Vec3b> it, end;
  for (it = _temp.begin<cv::Vec3b>(), end = _temp.end<cv::Vec3b>(); it != end; ++it) 
  {
      uchar &r = (*it)[0];
      uchar &g = (*it)[1];
      uchar &b = (*it)[2];
      cv::Vec3b &pixel_ = (*it);

      Eigen::Vector3d image_rgb_vec_;
      image_rgb_vec_ << r, g, b;

      // Multiply with transformation_matrix_
      Eigen::Vector3d image_rgb_vec_transformed_ = transformation_matrix_.transpose() * image_rgb_vec_; // <- try and .inv()

      r = cv::saturate_cast<uchar>(image_rgb_vec_transformed_(0));
      g = cv::saturate_cast<uchar>(image_rgb_vec_transformed_(1));
      b = cv::saturate_cast<uchar>(image_rgb_vec_transformed_(2));

      // std::cout << "Readen pixel : [" << image_rgb_vec_(0) << ", " 
      //                        << image_rgb_vec_(1) << ", " 
      //                        << image_rgb_vec_(2) << "]" 
      //                        << std::endl;
      // std::cout << "Transformmed pixel vector : [" << image_rgb_vec_transformed_(0) << ", " 
      //                        << image_rgb_vec_transformed_(1) << ", " 
      //                        << image_rgb_vec_transformed_(2) << "]" 
      //                        << std::endl;
                             
      // std::cout << "Setted pixel : " << pixel_ << std::endl;
      
      // std::cout << "Press any key for the next iteration" << std::endl;
      // std::cin.get();
  }

  //Convert the temp to 8Bit Unsigned char 3Channel, into the private Mat
  //Note: this is because imshow cannot display 32 or 64 bit floating point images
  this->RGB_image_ = _temp.clone();
}

void ConvertColorspace::LCCtooRGB(void)
{
  //Create a temp Mat, clone the original image
  cv::Mat _temp = this->LCC_image_.clone();
  
  //Convert the temp to 64Bit Floating Point, 3Channels
  //_temp.convertTo(_temp, CV_64FC3);

  cv::MatIterator_<cv::Vec3b> it, end;
  for (it = _temp.begin<cv::Vec3b>(), end = _temp.end<cv::Vec3b>(); it != end; ++it) 
  {
      uchar &l = (*it)[0];
      uchar &c1 = (*it)[2];
      uchar &c2 = (*it)[1];
      cv::Vec3b &pixel_ = (*it);

      Eigen::Vector3d image_rgb_vec_;
      image_rgb_vec_ << l, c1, c2;

      double rotation_angle_ = atan2(c2, c1);
      double rotation_angle_calculated_ = -1;
      double temp_ = -1;

      if (rotation_angle_ < (CV_PI / 3))
      {
        temp_ = (3 * rotation_angle_) / 2;
      }
      else if ((rotation_angle_ <= CV_PI) && (rotation_angle_ >= (CV_PI / 3)))
      {
        temp_ = (CV_PI / 2) + ((3 * (rotation_angle_ - (CV_PI / 3))) / 4);
      }

      rotation_angle_calculated_ = temp_ - rotation_angle_;

      //std::cout << "Ang Calc: " << rotation_angle_calculated_ << std::endl;

      Eigen::Matrix3d rotation_matrix_;
      rotation_matrix_ << 1,                               0,                                0,
                          0, cos(rotation_angle_calculated_), -sin(rotation_angle_calculated_),
                          0, sin(rotation_angle_calculated_),  cos(rotation_angle_calculated_);

      // Multiply with transformation_matrix_
      Eigen::Vector3d image_rgb_vec_rotated = rotation_matrix_.transpose() * image_rgb_vec_;

      //std::cout << "Getted pixel : " << pixel_ << std::endl;
      l  = image_rgb_vec_rotated(0);
      c1 = image_rgb_vec_rotated(1);
      c2 = image_rgb_vec_rotated(2);

      // std::cout << "Readen pixel : [" << image_rgb_vec_(0) << ", " 
      //                        << image_rgb_vec_(1) << ", " 
      //                        << image_rgb_vec_(2) << "]" 
      //                        << std::endl;
      // std::cout << "Transformmed pixel vector : [" << image_rgb_vec_rotated(0) << ", " 
      //                        << image_rgb_vec_rotated(1) << ", " 
      //                        << image_rgb_vec_rotated(2) << "]" 
      //                        << std::endl;
                             
      // std::cout << "Setted pixel : " << pixel_ << std::endl;
      
      // std::cout << "Press any key for the next iteration" << std::endl;
      // std::cin.get();
  }

  //Convert the temp to 8Bit Unsigned char 3Channel, into the private Mat
  //Note: this is because imshow cannot display 32 or 64 bit floating point images
  this->oRGB_image_ = _temp.clone();
}

void ConvertColorspace::oRGBtoLCC(void)
{
  //Create a temp Mat, clone the original image
  cv::Mat _temp = this->oRGB_image_.clone();
  
  //Convert the temp to 64Bit Floating Point, 3Channels
  //_temp.convertTo(_temp, CV_64FC3);

  cv::MatIterator_<cv::Vec3b> it, end;
  for (it = _temp.begin<cv::Vec3b>(), end = _temp.end<cv::Vec3b>(); it != end; ++it) 
  {
      uchar &r = (*it)[0];
      uchar &g = (*it)[1];
      uchar &b = (*it)[2];
      cv::Vec3b &pixel_ = (*it);

      Eigen::Vector3d image_rgb_vec_;
      image_rgb_vec_ << r, g, b;

      double rotation_angle_ = atan2(b, g);
      double rotation_angle_calculated_ = -1;
      double temp_ = -1;

      if (rotation_angle_ < (CV_PI / 2))
      {
        temp_ = (2 * rotation_angle_) / 3;
      }
      else if ((rotation_angle_ <= CV_PI) && (rotation_angle_ >= (CV_PI / 2)))
      {
        temp_ = (CV_PI / 3) + ((4 * (rotation_angle_ - (CV_PI / 2))) / 3);
      }

      rotation_angle_calculated_ = temp_ - rotation_angle_;

      //std::cout << "Ang Calc: " << rotation_angle_calculated_ << std::endl;

      Eigen::Matrix3d rotation_matrix_;
      rotation_matrix_ << 1,                               0,                                0,
                          0, cos(rotation_angle_calculated_), -sin(rotation_angle_calculated_),
                          0, sin(rotation_angle_calculated_),  cos(rotation_angle_calculated_);

      // Multiply with transformation_matrix_
      Eigen::Vector3d image_rgb_vec_rotated = rotation_matrix_.transpose() * image_rgb_vec_;

      //std::cout << "Getted pixel : " << pixel_ << std::endl;

      r = image_rgb_vec_rotated(0);
      g = image_rgb_vec_rotated(1);
      b = image_rgb_vec_rotated(2);

      // std::cout << "Readen pixel : [" << image_rgb_vec_(0) << ", " 
      //                        << image_rgb_vec_(1) << "]" 
      //                        << std::endl;
      // std::cout << "Transformmed pixel vector : [" << image_rgb_vec_rotated(0) << ", " 
      //                        << image_rgb_vec_rotated(1) << "]" 
      //                        << std::endl;
                             
      // std::cout << "Setted pixel : " << pixel_ << std::endl;
      
      // std::cout << "Press any key for the next iteration" << std::endl;
      //std::cin.get();
  }

  //Convert the temp to 8Bit Unsigned char 3Channel, into the private Mat
  //Note: this is because imshow cannot display 32 or 64 bit floating point images
  this->LCC_image_converted_ = _temp.clone();
}

void ConvertColorspace::splitChannels(cv::Mat image_)
{
  // std::vector<cv::Mat> chanels_;
  // cv::split(image_.clone(),chanels_);//split source

  // cv::imshow("Channel 1", chanels_[0]);
  // cv::imshow("Channel 2", chanels_[1]);
  // cv::imshow("Channel 3", chanels_[2]);
  cv::Mat original_image_ = image_.clone(); // <- here the oRGB

  cv::Mat Y ;// = cv::Mat::zeros(original_image_.rows, original_image_.cols, CV_8UC3 );
  cv::Mat Cr;// = cv::Mat::zeros(original_image_.rows, original_image_.cols, CV_8UC3 );
  cv::Mat Cb;// = cv::Mat::zeros(original_image_.rows, original_image_.cols, CV_8UC3 );

  // cv::Mat channels[] = { Y, Cr, Cb };
  // int from_to[] = {0,2, 1,4, 2,6 };
  
  // cv::mixChannels( &original_image_, 1, channels, 3, from_to, 3);

  Y = image_.clone();
  Y.reshape(1,Y.rows*Y.cols).col(1).setTo(cv::Scalar(0));
  Y.reshape(1,Y.rows*Y.cols).col(2).setTo(cv::Scalar(0));

  Cr = image_.clone();
  Cr.reshape(1,Cr.rows*Cr.cols).col(0).setTo(cv::Scalar(0));
  Cr.reshape(1,Cr.rows*Cr.cols).col(2).setTo(cv::Scalar(0));

  Cb = image_.clone();
  Cb.reshape(1,Cb.rows*Cb.cols).col(0).setTo(cv::Scalar(0));
  Cb.reshape(1,Cb.rows*Cb.cols).col(1).setTo(cv::Scalar(0));

  cvtColor(Y, Y, cv::COLOR_YCrCb2BGR);
  cvtColor(Cr, Cr, cv::COLOR_YCrCb2BGR);
  cvtColor(Cb, Cb, cv::COLOR_YCrCb2BGR);

  // Create 1280x480 mat for window
  cv::Mat result_(cv::Size(1080, 371), CV_8UC3, cv::Scalar(0, 0, 0));

  cvtColor(this->image_, original_image_, cv::COLOR_RGB2BGR);
  original_image_.copyTo(result_(cv::Rect(0, 0, 260, 371)));
  
  Y.copyTo(result_(cv::Rect(270, 0, 260, 371)));
  Cb.copyTo(result_(cv::Rect(540, 0, 260, 371)));
  Cr.copyTo(result_(cv::Rect(810, 0, 260, 371)));

  // Display big mat
  cv::namedWindow( "Channels", CV_WINDOW_NORMAL);
  cv::imshow("Channels", result_);

}

const cv::Vec3b ConvertColorspace::accessImgPixel(const cv::Mat image_, const std::size_t index_row_, const std::size_t index_col_) const noexcept
{
  cv::Vec3b temp_reverse_ = cv::Vec3b(image_.at<cv::Vec3b>(index_row_, index_col_)[2],
                                      image_.at<cv::Vec3b>(index_row_, index_col_)[1],
                                      image_.at<cv::Vec3b>(index_row_, index_col_)[0]);

  return temp_reverse_;
}

void ConvertColorspace::GammaCorrection(cv::Mat& image_, double scalar_)
{
  unsigned char gamma_correction_[256];

  for (int i = 0; i < 256; i++)
  {
    gamma_correction_[i] = cv::saturate_cast<uchar>(pow((float)(i / 255.0), scalar_) * 255.0f); 
  }

  cv::MatIterator_<cv::Vec3b> it, end;
  for (it = image_.begin<cv::Vec3b>(), end = image_.end<cv::Vec3b>(); it != end; ++it) 
  {
      uchar &r = (*it)[0];
      uchar &g = (*it)[1];
      uchar &b = (*it)[2];

      r = gamma_correction_[r];
      g = gamma_correction_[g];
      b = gamma_correction_[b];

  }
}

ConvertColorspace::~ConvertColorspace() {}