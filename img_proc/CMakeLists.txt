cmake_minimum_required (VERSION 3.0)

# General
###############################################################################

include_directories(
  "/usr/local/include"
  "/usr/include/eigen3"
)

# Executables
###############################################################################

link_directories(/usr/local/lib)

find_library(OpenCV REQUIRED)

add_library( convert_to_orgb
  SHARED
  RGB_to_oRGB/src/main.cpp
  RGB_to_oRGB/src/color_adjustment.cpp
  RGB_to_oRGB/src/convert_colorspace.cpp
)

# Link libraries
###############################################################################

target_link_libraries( convert_to_orgb
  opencv_core
  opencv_highgui
  opencv_imgcodecs
  opencv_imgproc
  opencv_calib3d
  opencv_dnn
  opencv_features2d
  opencv_flann
  opencv_ml
  opencv_objdetect
  opencv_photo
  opencv_shape
  opencv_stitching
  opencv_superres
  opencv_video
  opencv_videoio
  opencv_videostab
  opencv_viz
  opencv_aruco
  opencv_bgsegmo
  pencv_bioinspired
  opencv_ccalib
  opencv_cvv
  opencv_datasets
  opencv_dpm
  opencv_face
  opencv_fuzzy
  opencv_hdf
  opencv_img_hash
  opencv_line_descriptor
  opencv_optflow
  opencv_phase_unwrapping
  opencv_plot
  opencv_reg
  opencv_rgbd
  opencv_saliency
  opencv_stereo
  opencv_structured_light
  opencv_surface_matching
  opencv_text
  opencv_tracking
  opencv_xfeatures2d
  opencv_ximgproc
  opencv_xobjdetect
  opencv_xphoto
)

###############################################################################