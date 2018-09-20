package com.example.jimmyhalimi.snapmusic;

import android.app.Activity;
import android.content.Intent;
import android.graphics.Bitmap;
import android.os.Environment;
import android.provider.MediaStore;
import android.support.annotation.NonNull;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import com.camerakit.CameraKitView;

import android.util.Log;
import android.view.Menu;
import android.view.MenuItem;
import android.view.View;
import android.widget.Button;
import android.widget.ImageView;
import android.widget.Toast;

import java.io.File;
import java.io.FileOutputStream;
import java.io.IOException;

import org.opencv.android.OpenCVLoader;

public class MainPage extends AppCompatActivity {

  private CameraKitView cameraKitView;
  private Button photoButton, btnGallery;
  private ImageView ivImage;
  private int REQUEST_CAMERA = 0, SELECT_FILE = 1;
  private boolean camON = false;

  static {
    System.loadLibrary("native-lib");
    System.loadLibrary("opencv_java3");
  }

  @Override
  protected void onCreate(Bundle savedInstanceState) {
      super.onCreate(savedInstanceState);
      setContentView(R.layout.main_page);

      cameraKitView = findViewById(R.id.camera);
      photoButton = findViewById(R.id.photoButton);
      ivImage = (ImageView) findViewById(R.id.ivImage);
      photoButton.setOnClickListener(new View.OnClickListener() {
          @Override
          public void onClick(View v) {

              ivImage.setVisibility(View.INVISIBLE);
              cameraKitView.setVisibility(View.VISIBLE);
              if (camON){
                  cameraKitView.captureImage(new CameraKitView.ImageCallback() {
                      @Override
                      public void onImage(CameraKitView cameraKitView, final byte[] photo) {

                          // write the photo in device storage
                          File savedPhoto = new File(Environment.getExternalStorageDirectory(), System.currentTimeMillis() + ".jpg");
                          try {
                              FileOutputStream outputStream = new FileOutputStream(savedPhoto.getPath());
                              outputStream.write(photo);
                              outputStream.close();

                            if (!OpenCVLoader.initDebug()) {
                              //textView.setText(textView.getText() + "\n OpenCVLoader.initDebug(), not working.");
                            } else {
                              //textView.setText(textView.getText() + "\n OpenCVLoader.initDebug(), WORKING.");
                              //DRS 20160822c Added 1
                              //textView.setText(textView.getText() + "\n" + validate(0L, 0L));
                            }

                              Toast.makeText(MainPage.this, "Saved " + savedPhoto, Toast.LENGTH_SHORT).show();

                          } catch (java.io.IOException e) {
                              e.printStackTrace();
                              Log.e("CKDemo", "Exception in photo callback");
                          }
                      }
                  });
          }
          camON = true;
          }
      });


      btnGallery = (Button) findViewById(R.id.btnGallery);
      btnGallery.setOnClickListener(new View.OnClickListener() {
          @Override
          public void onClick(View v) {

              galleryIntent();
              cameraKitView.setVisibility(View.INVISIBLE);
              ivImage.setVisibility(View.VISIBLE);
              camON = false;
          }
      });
  }

  private void galleryIntent()
  {
      Intent intent = new Intent();
      intent.setType("image/*");
      intent.setAction(Intent.ACTION_GET_CONTENT);//
      startActivityForResult(Intent.createChooser(intent, "Select File"),SELECT_FILE);
  }

  @Override
  public void onActivityResult(int requestCode, int resultCode, Intent data) {
      super.onActivityResult(requestCode, resultCode, data);

      if (resultCode == Activity.RESULT_OK) {
          if (requestCode == SELECT_FILE)
              onSelectFromGalleryResult(data);
          //  else if (requestCode == REQUEST_CAMERA)
          //    onCaptureImageResult(data);
      }
  }

  @SuppressWarnings("deprecation")
  private void onSelectFromGalleryResult(Intent data) {

      Bitmap bm=null;
      if (data != null) {
          try {
              bm = MediaStore.Images.Media.getBitmap(getApplicationContext().getContentResolver(), data.getData());
          } catch (IOException e) {
              e.printStackTrace();
          }
      }
      // bm is the image from gallery
      ivImage.setImageBitmap(bm);
  }


  @Override
  protected void onResume() {
      super.onResume();
      cameraKitView.onResume();
  }

  @Override
  protected void onPause() {
      cameraKitView.onPause();
      super.onPause();
  }

  @Override
  public void onRequestPermissionsResult(int requestCode, @NonNull String[] permissions, @NonNull int[] grantResults) {
      super.onRequestPermissionsResult(requestCode, permissions, grantResults);
      cameraKitView.onRequestPermissionsResult(requestCode, permissions, grantResults);
  }

  @Override
  public boolean onCreateOptionsMenu(Menu menu) {

      getMenuInflater().inflate(R.menu.menu,menu);
      return true;
  }
  @Override
  public boolean onOptionsItemSelected(MenuItem item) {

      return super.onOptionsItemSelected(item);
  }

  //C++ Declaration
  public native String stringFromJNI();

  //DRS 20160822c - Added 1
  public native String validate(long matAddrGr, long matAddrRgba);
}