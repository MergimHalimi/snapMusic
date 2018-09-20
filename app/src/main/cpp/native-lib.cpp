#include "native-lib.h"

//JNIEXPORT jstring JNICALL
//Java_com_example_jimmyhalimi_snapmusic_MainActivity_getList(JNIEnv *javaEnvironment , jobject __unused obj, cv::Mat image)

JNIEXPORT jobject JNICALL
  Java_com_example_jimmyhalimi_snapmusic_MainActivity_getList(JNIEnv *env , jclass, cv::Mat image) {

    // Find Class for Vector ImageProcessor
    jclass clsVec = env->FindClass("java/util/Vector");

    // Get its constructor method
    jmethodID constructor = env->GetMethodID(clsVec, "<init>", "()V");

    // Create new Vector object
    jobject objVec = env->NewObject(clsVec, constructor, "");

    // Get addElement method
    jmethodID vecAdd = env->GetMethodID(clsVec, "addElement", "(Ljava/lang/Object;)V");

    //Part to call the image processor class
    ImageProcessor image_;

    for (size_t i; i<image_.getData().size(); i++)
    {
      // Add new string (created locally)
      jstring retStr = env->NewStringUTF(image_.getData()[i].c_str());
      env->CallVoidMethod(objVec, vecAdd, retStr);
    }
    ///////////////////////////////////////////////

    // Add string from parameter
    //env->CallVoidMethod(objVec, vecAdd, str);

    // Always release local references.
    env->DeleteLocalRef(clsVec);

    return objVec;
  }