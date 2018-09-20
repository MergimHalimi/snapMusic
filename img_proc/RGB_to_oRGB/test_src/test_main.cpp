/****************************************************************
   PROGRAM:    RGB to oRGB Unit Tests
   AUTHOR:     Mergim Halimi   m.halimi@live.com
   DUE DATE:   

   FUNCTION:   This app is used to test all the functions in
               RGB to oRGB application

   NOTES:      Using Google Test Framework
****************************************************************/

#include <string>

#include "gtest/gtest.h"  /* GoogleTest C++ Framework HEADER    */

std::string tests_data_path_;

const std::string TestsDataPath (void) {

  std::string tests_main_path_ = __FILE__;
  //std::cout << "Splitting: " << tests_main_path_ << '\n';
  std::size_t found = tests_main_path_.find_last_of("/\\");
  tests_main_path_ = tests_main_path_.substr(0,found);
  found = tests_main_path_.find_last_of("/\\");
  tests_main_path_ = tests_main_path_.substr(0,found);
  found = tests_main_path_.find_last_of("/\\");
  //std::cout << " path: " << tests_main_path_.substr(0,found) << '\n';
  //std::cout << " file: " << tests_main_path_.substr(found+1) << '\n';

  return tests_main_path_.substr(0,found) + "/Test_Files";
}

int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);

    // std::cout << "Arguments : \n";
    // for (size_t i = 0; i < argc; i++){
    //   std::cout << "Argv [" << i << "] : " << argv[i] << std::endl;
    // }

    //std::cout << TestsDataPath() << std::endl;

    tests_data_path_ = TestsDataPath();

    return RUN_ALL_TESTS();
}

