package com.example.jimmyhalimi.snapmusic;

import android.app.AlertDialog;
import android.content.Context;
import android.content.DialogInterface;
import android.support.annotation.MainThread;

import org.opencv.core.Mat;
import org.opencv.imgcodecs.Imgcodecs;

import java.util.Vector;

import com.example.jimmyhalimi.snapmusic.MainPage;

class ImageProcessingThread extends Thread {

  private String  path;
  ImageProcessingThread(String path) {
    this.path = path;
  }

  public void run() {
    loadImageInImageProcessor(path);
  }

  private void loadImageInImageProcessor(String path)
  {
      // Call Native c++  ------------------------------------------------------------------------------------------

      Mat image_ = Imgcodecs.imread(path, Imgcodecs.CV_LOAD_IMAGE_COLOR);

      String _buf = "Path: " + path + "\nResults: \n";
      Vector result_list_ = getList(image_.getNativeObjAddr());

      for (int i = 0; i < result_list_.size(); i++) {
        _buf = _buf + result_list_.get(i).toString() + ", \n";
      }

      // Call Native c++  ------------------------------------------------------------------------------------------

      AlertDialog.Builder builder_ = new AlertDialog.Builder(MainPage.getActivity());

      builder_.setMessage(_buf);
      builder_.setCancelable(true);
      builder_.setPositiveButton("OK",

        new DialogInterface.OnClickListener() {
          public void onClick(DialogInterface dialog, int id) {
            dialog.cancel();
            MainPage.getActivity().setFlags(false);
          }
        });

      AlertDialog alert2 = builder_.create();
      alert2.show();
  }

  //C++ function declaration
  public native Vector getList(long matAddr);
}