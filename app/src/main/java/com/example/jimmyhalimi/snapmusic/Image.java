package com.example.jimmyhalimi.snapmusic;

import android.app.AlertDialog;
import android.content.DialogInterface;
import android.content.Intent;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.net.Uri;
import android.provider.MediaStore;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.widget.EditText;
import android.widget.ImageView;
import android.widget.ProgressBar;
import android.widget.Toast;

import org.opencv.core.Mat;
import org.opencv.imgcodecs.Imgcodecs;

import java.io.IOException;
import java.util.Vector;

public class Image extends AppCompatActivity {

  ImageView ivImage;
  private ProgressBar spinner;
  private ObservableBoolean is_processing_ = new ObservableBoolean();
  private String _buf, image_path_;


  @Override
  protected void onCreate(Bundle savedInstanceState)
  {
    super.onCreate(savedInstanceState);
    setContentView(R.layout.activity_image);

    ivImage = (ImageView) findViewById(R.id.ivImage);

    // Call Native c++ thread -----------------------------------------------------------------------------------------

    final image_processing_thread img_processor = new image_processing_thread() {
      @Override
      public void doWork() {
        loadImageInImageProcessor(image_path_);
      }
    };

    image_processing_listener listener = new image_processing_listener() {
      @Override
      public void threadComplete(Runnable runner) {
        AlertDialog.Builder builder_ = new AlertDialog.Builder(Image.this);

        builder_.setMessage(_buf);
        builder_.setCancelable(true);
        builder_.setPositiveButton("OK", new DialogInterface.OnClickListener() {
          public void onClick(DialogInterface dialog, int id)
          {
            setFlags(false);
            Intent return_home_ = new Intent(Image.this, Home.class);
            startActivity(return_home_);
            dialog.cancel();
          }
        });

        AlertDialog alert2 = builder_.create();
        alert2.show();
      }
    };

    img_processor.addListener(listener);

    spinner = (ProgressBar) findViewById(R.id.progress_bar_);
    spinner.setVisibility(View.GONE);
    is_processing_.setOnBooleanChangeListener(new ObservableBoolean.OnBooleanChangeListener()
    {
      @Override
      public void onBooleanChanged(boolean newValue)
      {
        if (newValue == false)
        {
          _buf = null;
          image_path_ = null;
          spinner.setVisibility(View.GONE);
          onResume();
        }
        else if (newValue == true)
        {
          spinner.setVisibility(View.VISIBLE);
          spinner.bringToFront();
          onPause();
          if(image_path_ != null)
          {
            img_processor.run();
          }
        }
      }
    });
    // Call Native c++ thread -----------------------------------------------------------------------------------------

    if(getIntent().hasExtra("data1"))
    {
      Bundle bd = getIntent().getExtras();
      Intent data = bd.getParcelable("data1");
      Bitmap bm = null;
      try
      {
        bm = MediaStore.Images.Media.getBitmap(getApplicationContext().getContentResolver(), data.getData());
      } catch (IOException e) {
        e.printStackTrace();
      }
      String gallery_path = getIntent().getStringExtra("galleryPath");
      ivImage.setImageBitmap(bm);

      this.image_path_ = gallery_path;
      setFlags(true);
    }

    if(getIntent().hasExtra("path1"))
    {
      String image_path_ = getIntent().getStringExtra("path1");
      Bitmap myBitmap = BitmapFactory.decodeFile(image_path_);
      ivImage.setImageBitmap(myBitmap);

      this.image_path_ = image_path_;
      setFlags(true);
    }
  }

  public void setFlags(boolean flag_)
  {
    is_processing_.triggerBooleanListener(flag_);
  }

  private void loadImageInImageProcessor(String path)
  {
    // Call Native c++  ------------------------------------------------------------------------------------------

    Mat image_ = Imgcodecs.imread(path, Imgcodecs.CV_LOAD_IMAGE_COLOR);

    this._buf = "Path: " + path + "\nResults: \n";
    Vector result_list_ = getList(image_.getNativeObjAddr());

    String processed_img_path_ = image_path_.replace("snappedImg", "processedImg");
    processed_img_path_ = processed_img_path_.replace(".jpg", "_processed.jpg");
    Imgcodecs.imwrite(processed_img_path_, image_);
    Toast.makeText(Image.this, "Saved " + processed_img_path_, Toast.LENGTH_SHORT).show();
    sendBroadcast(new Intent(Intent.ACTION_MEDIA_SCANNER_SCAN_FILE, Uri.parse("file://" + processed_img_path_)));

    for (int i = 0; i < result_list_.size(); i++) {
      _buf = _buf + result_list_.get(i).toString() + ", \n";
    }

    // Call Native c++  ------------------------------------------------------------------------------------------
  }

  static
  {
    System.loadLibrary("native-lib");
    System.loadLibrary("opencv_java3");
  }

  //C++ function declaration
  public native Vector getList(long matAddr);
}
