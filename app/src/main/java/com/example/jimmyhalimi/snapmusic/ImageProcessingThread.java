package com.example.jimmyhalimi.snapmusic;

import android.app.AlertDialog;
import android.content.Context;
import android.content.DialogInterface;
import android.support.annotation.MainThread;

import org.opencv.core.Mat;
import org.opencv.imgcodecs.Imgcodecs;

import java.util.Vector;



class ImageProcessingThread extends Thread {

//  public interface ThreadCompleteListener {
//    void notifyOfThreadComplete(final Thread thread);
//  }

  private String  path;
  private String _buf;
  ImageProcessingThread(String path) {
    this.path = path;
  }

  public void run() {
    this.loadImageInImageProcessor(path);
  }

  private void loadImageInImageProcessor(String path)
  {
      // Call Native c++  ------------------------------------------------------------------------------------------

      Mat image_ = Imgcodecs.imread(path, Imgcodecs.CV_LOAD_IMAGE_COLOR);

      _buf = "Path: " + path + "\nResults: \n";
      Vector result_list_ = getList(image_.getNativeObjAddr());

      for (int i = 0; i < result_list_.size(); i++) {
        _buf = _buf + result_list_.get(i).toString() + ", \n";
      }

      //MainPage.getActivity().threadFinished(_buf);
  }

  public String getBuff()
  {
    return _buf;
  }

//  public final void addListener(final ThreadCompleteListener listener) {
//    listeners.add(listener);
//  }

  static {
    System.loadLibrary("native-lib");
    System.loadLibrary("opencv_java3");
  }

  //C++ function declaration
  public native Vector getList(long matAddr);
}