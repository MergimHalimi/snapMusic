#ifndef ___CPLUSPLUS___
#define ___CPLUSPLUS___

#include <jni.h>
#include "img_processor.h"

extern "C" {

/*
 * Class:     com_example_jimmyhalimi_snapmusic_MainActivity
 * Method:    Validate
 * Signature: (Ljava/lang/String;)Ljava/util/Vector;
 */
JNIEXPORT jobject JNICALL
  Java_com_example_jimmyhalimi_snapmusic_MainActivity_getList
      (JNIEnv *, jclass, cv::Mat image);
  }

#endif //___CPLUSPLUS___