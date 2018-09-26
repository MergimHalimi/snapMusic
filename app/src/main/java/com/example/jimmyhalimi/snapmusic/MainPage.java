package com.example.jimmyhalimi.snapmusic;

import android.Manifest;
import android.app.Activity;
import android.app.AlertDialog;
import android.content.DialogInterface;
import android.content.Intent;
import android.content.pm.PackageManager;
import android.graphics.Bitmap;
import android.net.Uri;
import android.os.Build;
import android.os.Environment;
import android.provider.MediaStore;
import android.support.annotation.NonNull;
import android.support.v4.app.ActivityCompat;
import android.support.v4.content.ContextCompat;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;

import com.camerakit.CameraKit;
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
import java.util.Vector;

import org.json.JSONObject;
import org.opencv.android.OpenCVLoader;
import org.opencv.core.Mat;
import org.opencv.imgcodecs.Imgcodecs;

public class MainPage extends AppCompatActivity
{

  private CameraKitView cameraKitView;
  private Button photoButton, btnGallery, btnChange, btnFlash;
  private ImageView ivImage;
  private int SELECT_FILE = 1;
  private static final String TAG = "MainPage";
  private static final int REQUEST_CODE = 1;
  private boolean camON = true, front = false, flashOn = false ;

  @Override
  protected void onCreate(Bundle savedInstanceState)
  {
    super.onCreate(savedInstanceState);
    setContentView(R.layout.main_page);

    verifyPermissions();

    cameraKitView = findViewById(R.id.camera);
    photoButton = findViewById(R.id.photoButton);
    ivImage = (ImageView) findViewById(R.id.ivImage);
    photoButton.setOnClickListener(new View.OnClickListener()
    {
      @Override
      public void onClick(View v) 
      {
        ivImage.setVisibility(View.INVISIBLE);
        cameraKitView.setVisibility(View.VISIBLE);
        btnChange.setVisibility(View.VISIBLE);
        btnFlash.setVisibility(View.VISIBLE);
        if (camON)
        {
          cameraKitView.captureImage(new CameraKitView.ImageCallback()
          {

            @Override
            public void onImage(CameraKitView cameraKitView, final byte[] photo)
            {
              // write the photo in device storage
              File savedPhoto = new File(Environment.getExternalStorageDirectory(), System.currentTimeMillis() + ".jpg");

              try
              {
                FileOutputStream outputStream = new FileOutputStream(savedPhoto.getPath());
                outputStream.write(photo);
                outputStream.close();

                Toast.makeText(MainPage.this, "Saved " + savedPhoto, Toast.LENGTH_SHORT).show();
                sendBroadcast(new Intent(Intent.ACTION_MEDIA_SCANNER_SCAN_FILE, Uri.parse("file://" + savedPhoto.getAbsolutePath())));


              }
              catch (java.io.IOException e)
              {
                e.printStackTrace();
                Log.e("CKDemo", "Exception in photo callback");
              }

              // Call Native c++  ------------------------------------------------------------------------------------------

              Mat image_ = Imgcodecs.imread(savedPhoto.toString(), Imgcodecs.CV_LOAD_IMAGE_COLOR);

              AlertDialog.Builder builder_ = new AlertDialog.Builder(MainPage.this);

              String _buf = "";
              Vector result_list_ = getList(image_.getNativeObjAddr());

              for (int i = 0; i < result_list_.size(); i++)
              {
                _buf = _buf + result_list_.get(i).toString() + ", \n";
              }

             builder_.setMessage(_buf);
             builder_.setCancelable(true);
             builder_.setPositiveButton( "OK", 
                                          new DialogInterface.OnClickListener() 
                                          {
                                            public void onClick(DialogInterface dialog, int id)
                                            {
                                              dialog.cancel();
                                            }
                                          });

                AlertDialog alert2 =builder_.create();
                alert2.show();

                // Call Native c++  ------------------------------------------------------------------------------------------
            }
          });
        }
        camON = true;
      }
    });



    btnChange = (Button)findViewById(R.id.btnChange);
    btnChange.setOnClickListener(new View.OnClickListener()
    {
      @Override
      public void onClick(View v) {
        if(front == false)
        {
          cameraKitView.setFacing(CameraKit.FACING_FRONT);

          front = true;
        }
        else
        {
          cameraKitView.setFacing(CameraKit.FACING_BACK);
          front = false;
        }
      }
    });

    btnGallery = (Button) findViewById(R.id.btnGallery);
    btnGallery.setOnClickListener(new View.OnClickListener()
    {
      @Override
      public void onClick(View v) {

          galleryIntent();
          cameraKitView.setVisibility(View.INVISIBLE);
          ivImage.setVisibility(View.VISIBLE);
          camON = false;
          btnChange.setVisibility(View.GONE);
          btnFlash.setVisibility(View.GONE);

      }
    });


    btnFlash = (Button)findViewById(R.id.btnFlash);
    btnFlash.setBackgroundResource(R.drawable.flashnot);
    btnFlash.setOnClickListener(new View.OnClickListener() {
        @Override
        public void onClick(View v) {

            if(flashOn == false)
            {
              cameraKitView.setFlash(CameraKit.FLASH_ON);
              flashOn = true;

              btnFlash.setBackgroundResource(R.drawable.flash);
            }
            else
            {
                cameraKitView.setFlash(CameraKit.FLASH_OFF);
                flashOn = false;

                btnFlash.setBackgroundResource(R.drawable.flashnot);
            }
        }
    });
  }

  private void galleryIntent()
  {
      Intent intent = new Intent();
      intent.setType("image/*");
      intent.setAction(Intent.ACTION_GET_CONTENT);

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
      verifyPermissions();


  }



  private void verifyPermissions(){
        Log.d(TAG, "verifyPermissions: asking user for permissions");
        String[] permissions = {Manifest.permission.READ_EXTERNAL_STORAGE,
                Manifest.permission.WRITE_EXTERNAL_STORAGE,
                Manifest.permission.CAMERA};

        if(ContextCompat.checkSelfPermission(this.getApplicationContext(),
                permissions[0]) == PackageManager.PERMISSION_GRANTED
                && ContextCompat.checkSelfPermission(this.getApplicationContext(),
                permissions[1]) == PackageManager.PERMISSION_GRANTED
                && ContextCompat.checkSelfPermission(this.getApplicationContext(),
                permissions[2]) == PackageManager.PERMISSION_GRANTED){

        }else{
            ActivityCompat.requestPermissions(MainPage.this,
                    permissions,
                    REQUEST_CODE);
        }
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

  static {
    System.loadLibrary("native-lib");
    System.loadLibrary("opencv_java3");
  }

  //C++ function declaration
  public native Vector getList(long matAddr);
}