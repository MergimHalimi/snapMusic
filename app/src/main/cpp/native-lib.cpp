#include "native-lib.h"

extern "C" {
JNIEXPORT jobject JNICALL
  Java_com_example_jimmyhalimi_snapmusic_MainPage_getList(JNIEnv *env , jclass, long addrInputImage) {

    // Find Class for Vector ImageProcessor
    jclass clsVec = env->FindClass("java/util/Vector");

    // Get its constructor method
    jmethodID constructor = env->GetMethodID(clsVec, "<init>", "()V");

    // Create new Vector object
    jobject objVec = env->NewObject(clsVec, constructor, "");

    // Get addElement method
    jmethodID vecAdd = env->GetMethodID(clsVec, "addElement", "(Ljava/lang/Object;)V");

    ///////////////////////////////////////////////////////////////////////////////////////////////
    //Part to call the image processor class
    cv::Mat* pInputImage = reinterpret_cast<cv::Mat*>(addrInputImage);

    ImageProcessor image_;

    image_.initialize(*pInputImage);

    image_.runProcessor();

    std::vector<std::string> _buf = image_.getData();

    for (size_t i=0; i<_buf.size(); i++)
    {
      // Add new string (created locally)
      jstring retStr = env->NewStringUTF(_buf[i].c_str());
      env->CallVoidMethod(objVec, vecAdd, retStr);
    }
    ///////////////////////////////////////////////////////////////////////////////////////////////

    // Always release local references.
    env->DeleteLocalRef(clsVec);

    return objVec;
  }
}