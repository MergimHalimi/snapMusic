package com.example.jimmyhalimi.snapmusic;

import android.Manifest;
import android.app.ActionBar;
import android.app.Activity;
import android.app.AlertDialog;


import android.content.DialogInterface;
import android.content.Intent;
import android.content.pm.PackageManager;
import android.content.res.Configuration;
import android.database.Cursor;
import android.graphics.Bitmap;

import android.net.Uri;
import android.os.Build;
import android.os.Environment;
import android.provider.MediaStore;
import android.support.annotation.NonNull;
import android.support.design.widget.NavigationView;
import android.support.v4.app.ActivityCompat;


import android.support.v4.content.ContextCompat;
import android.support.v4.widget.DrawerLayout;
import android.support.v7.app.ActionBarDrawerToggle;
import android.os.Bundle;

import com.camerakit.CameraKit;
import com.camerakit.CameraKitView;

import android.support.v7.app.AppCompatActivity;
import android.support.v7.widget.Toolbar;
import android.util.Log;
import android.view.Menu;
import android.view.MenuItem;
import android.view.View;
import android.widget.ArrayAdapter;
import android.widget.Button;
import android.widget.ImageView;
import android.widget.ListView;

import android.widget.Toast;
import android.widget.ProgressBar;

import java.io.File;
import java.io.FileOutputStream;
import java.io.IOException;
import java.util.Vector;


import org.opencv.android.OpenCVLoader;
import org.opencv.core.Mat;
import org.opencv.imgcodecs.Imgcodecs;

public class MainPage extends AppCompatActivity
        implements NavigationView.OnNavigationItemSelectedListener
{

  private CameraKitView cameraKitView;
  private Button photoButton, btnGallery, btnChange, btnFlash;
  private ImageView ivImage;
  private int SELECT_FILE = 1;
  private static final String TAG = "MainPage";
  private static final int REQUEST_CODE = 1;
  private boolean camON = true, front = false, flashOn = false;
  private String image_path_;
  private ProgressBar spinner;
  private ObservableBoolean is_processing_ = new ObservableBoolean();
  private String _buf;
    String[] listArray;
    ListView drawerListView;
    ActionBarDrawerToggle mActionBarDrawerToggle;
    DrawerLayout mDrawerLayout;

  @Override
  protected void onCreate(Bundle savedInstanceState)
  {
    super.onCreate(savedInstanceState);
    setContentView(R.layout.main_page);

    verifyPermissions();

      Toolbar toolbar = (Toolbar) findViewById(R.id.toolbar);
      setSupportActionBar(toolbar);

      DrawerLayout drawer = (DrawerLayout) findViewById(R.id.drawer_layout);
      ActionBarDrawerToggle toggle = new ActionBarDrawerToggle(
              this, drawer, toolbar, R.string.navigation_drawer_open, R.string.navigation_drawer_close);
      drawer.addDrawerListener(toggle);
      toggle.syncState();

      NavigationView navigationView = (NavigationView) findViewById(R.id.nav_view);
      navigationView.setNavigationItemSelectedListener(this);

     // getSupportActionBar().setDisplayOptions(ActionBar.DISPLAY_SHOW_CUSTOM);
      getSupportActionBar().setCustomView(R.layout.abs_layout);
      getSupportActionBar().setDisplayHomeAsUpEnabled(true);
      getSupportActionBar().setHomeButtonEnabled(true);

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
            public void onImage(final CameraKitView cameraKitView, final byte[] photo)
            {
              // write the photo in device storage
              File savedPhoto = new File(Environment.getExternalStoragePublicDirectory(Environment.DIRECTORY_DCIM),System.currentTimeMillis() + ".jpg");
              try
              {
                FileOutputStream outputStream = new FileOutputStream(savedPhoto.getAbsolutePath());
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

              image_path_ = savedPhoto.getAbsolutePath();
            }

          });
         // loadImageInImageProcessor(image_path_);
        }
        camON = true;
        setFlags(true);
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

          ivImage.setImageBitmap(null);
          galleryIntent();
          cameraKitView.setVisibility(View.INVISIBLE);
          ivImage.setVisibility(View.VISIBLE);
          camON = false;
          btnChange.setVisibility(View.GONE);
          btnFlash.setVisibility(View.GONE);

      }
    });


    btnFlash = (Button)findViewById(R.id.btnFlash);
    btnFlash.setBackgroundResource(R.drawable.ic_flash_off);
    btnFlash.setOnClickListener(new View.OnClickListener() {
        @Override
        public void onClick(View v) {

            if(flashOn == false)
            {
              cameraKitView.setFlash(CameraKit.FLASH_ON);
              flashOn = true;

              btnFlash.setBackgroundResource(R.drawable.ic_flash_on);
            }
            else
            {
                cameraKitView.setFlash(CameraKit.FLASH_OFF);
                flashOn = false;

                btnFlash.setBackgroundResource(R.drawable.ic_flash_off);
            }
        }
    });

    ///////////////////////////////

    final image_processing_thread img_processor = new image_processing_thread() {
      @Override
      public void doWork() {
        loadImageInImageProcessor(image_path_);
      }
    };

    image_processing_listener listener = new image_processing_listener() {
      @Override
      public void threadComplete(Runnable runner) {
        AlertDialog.Builder builder_ = new AlertDialog.Builder(MainPage.this);

        builder_.setMessage(_buf);
        builder_.setCancelable(true);
        builder_.setPositiveButton("OK", new DialogInterface.OnClickListener() {
          public void onClick(DialogInterface dialog, int id)
          {
            dialog.cancel();
            _buf = null;
            setFlags(false);
          }
        });

        AlertDialog alert2 = builder_.create();
        alert2.show();
      }
    };

    img_processor.addListener(listener);

    spinner = (ProgressBar) findViewById(R.id.progress_bar_);
    is_processing_.setOnBooleanChangeListener(new ObservableBoolean.OnBooleanChangeListener()
    {
      @Override
      public void onBooleanChanged(boolean newValue)
      {
        if (newValue == false)
        {
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
    //////////////////////////////////
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

      //loadImageInImageProcessor("");

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

  public void verifyPermissions(){
        Log.d(TAG, "verifyPermissions: asking user for permissions");
        String[] permissions = {Manifest.permission.READ_EXTERNAL_STORAGE,
                Manifest.permission.WRITE_EXTERNAL_STORAGE,
                Manifest.permission.CAMERA};

        if(ContextCompat.checkSelfPermission(this.getApplicationContext(),
                permissions[0]) == PackageManager.PERMISSION_GRANTED
                && ContextCompat.checkSelfPermission(this.getApplicationContext(),
                permissions[1]) == PackageManager.PERMISSION_GRANTED
               ){

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

      if (item.getItemId()==R.id.Settings)
      {
          //Intent objS = new Intent(MainPage.this,Settings.class);
          //startActivity(objS);
      }
      return super.onOptionsItemSelected(item);
  }

    @SuppressWarnings("StatementWithEmptyBody")
    @Override
    public boolean onNavigationItemSelected(MenuItem item) {
        // Handle navigation view item clicks here.
        int id = item.getItemId();

        if (id == R.id.nav_home) {

        } else if (id == R.id.nav_gallery) {

       } else if (id == R.id.nav_settings) {
            Intent objS1 = new Intent(MainPage.this,Settings.class);
            startActivity(objS1);

  }


        return true;
    }

    public void setFlags(boolean flag_) {
      is_processing_.triggerBooleanListener(flag_);
    }

  private void loadImageInImageProcessor(String path)
  {
    // Call Native c++  ------------------------------------------------------------------------------------------

    Mat image_ = Imgcodecs.imread(path, Imgcodecs.CV_LOAD_IMAGE_COLOR);

    this._buf = "Path: " + path + "\nResults: \n";
    Vector result_list_ = getList(image_.getNativeObjAddr());

    for (int i = 0; i < result_list_.size(); i++) {
      _buf = _buf + result_list_.get(i).toString() + ", \n";
    }

    // Call Native c++  ------------------------------------------------------------------------------------------
  }

  //C++ function declaration
  static
  {
    System.loadLibrary("native-lib");
    System.loadLibrary("opencv_java3");
  }

  //C++ function declaration
  public native Vector getList(long matAddr);
}