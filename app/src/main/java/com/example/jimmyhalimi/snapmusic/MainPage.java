package com.example.jimmyhalimi.snapmusic;

import android.Manifest;
import android.app.Activity;
import android.app.AlertDialog;

import android.app.FragmentTransaction;
import android.content.DialogInterface;
import android.content.Intent;
import android.content.pm.PackageManager;
import android.content.res.AssetManager;
import android.content.res.Configuration;
import android.database.Cursor;
import android.graphics.Bitmap;
import android.graphics.Camera;
import android.net.Uri;
import android.os.Build;
import android.os.Environment;
import android.provider.MediaStore;
import android.support.annotation.NonNull;
import android.support.v4.app.ActivityCompat;
import android.app.Fragment;
import android.app.FragmentManager;

import android.support.v4.content.ContextCompat;
import android.support.v4.widget.DrawerLayout;
import android.support.v7.app.ActionBarDrawerToggle;
import android.os.Bundle;

import com.camerakit.CameraKit;
import com.camerakit.CameraKitView;

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

import java.io.BufferedOutputStream;
import java.io.File;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.util.Vector;


import org.opencv.android.OpenCVLoader;
import org.opencv.core.Mat;
import org.opencv.imgcodecs.Imgcodecs;

public class MainPage extends Activity
{

  private CameraKitView cameraKitView;
  private Button photoButton, btnGallery, btnChange, btnFlash;
  private ImageView ivImage;
  private int SELECT_FILE = 1;
  private static final String TAG = "MainPage";
  private static final int REQUEST_CODE = 1;
  private boolean camON = true, front = false, flashOn = false;
  private String image_path_, dir = "assets/datasets";
  private ProgressBar spinner;
  private ObservableBoolean is_processing_ = new ObservableBoolean();
  private static MainPage main_instace_;

    String[] listArray;
    ListView drawerListView;
    ActionBarDrawerToggle mActionBarDrawerToggle;
    DrawerLayout mDrawerLayout;

  @Override
  protected void onCreate(Bundle savedInstanceState)
  {
    super.onCreate(savedInstanceState);
    setContentView(R.layout.main_page);
    main_instace_ = this;



    verifyPermissions();
      copyAssets();

      listArray = getResources().getStringArray(R.array.listArray);
      drawerListView = (ListView)findViewById(R.id.drawerLayoutListId);
      mDrawerLayout = (DrawerLayout) findViewById(R.id.drawerLayoutId);

      drawerListView.setAdapter(new ArrayAdapter<String>(this, android.R.layout.simple_list_item_activated_1, listArray));

      //Create Drawer Toggle
      mActionBarDrawerToggle = new ActionBarDrawerToggle(this, mDrawerLayout, R.string.open_drawer, R.string.close_drawer){
          @Override
          public void onDrawerOpened(View drawerView) {
              super.onDrawerOpened(drawerView);
          }

          @Override
          public void onDrawerClosed(View drawerView) {
              super.onDrawerClosed(drawerView);
          }
      };
      mDrawerLayout.setDrawerListener(mActionBarDrawerToggle);

      //Enable the drawer to open and close
      getActionBar().setDisplayHomeAsUpEnabled(true);
      getActionBar().setHomeButtonEnabled(true);

      FragmentTransaction ft = getFragmentManager().beginTransaction();
      TopFragment tf = new TopFragment();
      //Changed to add Fragment Name
      //ft.replace(R.id.frameLayoutId, tf);
      ft.replace(R.id.frameLayoutId, tf, "visible_fragment");
      ft.setTransition(FragmentTransaction.TRANSIT_FRAGMENT_FADE);
      ft.addToBackStack(null);
      ft.commit();

      //Add a backStack Listener

      getFragmentManager().addOnBackStackChangedListener(new FragmentManager.OnBackStackChangedListener() {
          @Override
          public void onBackStackChanged() {
              Fragment currentBackStackFragment = getFragmentManager().findFragmentByTag("visible_fragment");
              if(currentBackStackFragment instanceof TopFragment){
                  //Add Code
              }
          }
      });


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
    //ObservableBoolean is_processing_ = new ObservableBoolean();
    //setFlags(processing_);

    spinner = (ProgressBar) findViewById(R.id.progress_bar_);

    is_processing_.setOnBooleanChangeListener(new ObservableBoolean.OnBooleanChangeListener()
    {
      @Override
      public void onBooleanChanged(boolean newValue) {

        if (newValue == false)
        {
          image_path_ = null;
          spinner.setVisibility(View.GONE);
          cameraKitView.onResume();
          //onResume();
        }
        else if (newValue == true)
        {
          spinner.setVisibility(View.VISIBLE);
          spinner.bringToFront();
          cameraKitView.onPause();
          if(image_path_ != null)
          {
            ImageProcessingThread img_procc_ = new ImageProcessingThread(image_path_);
            img_procc_.start();
            try
            {
              img_procc_.join();

            }
            catch (InterruptedException e)
            {
              e.printStackTrace();
            }
            threadFinished(img_procc_.getBuff());

          }
          //onPause();
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

      if(mActionBarDrawerToggle.onOptionsItemSelected(item)){
          return true;
      }

      if (item.getItemId()==R.id.Settings)
      {
          //Intent objS = new Intent(MainPage.this,Settings.class);
          //startActivity(objS);
      }
      return super.onOptionsItemSelected(item);
  }

    @Override
    protected void onPostCreate(Bundle savedInstanceState) {
        super.onPostCreate(savedInstanceState);
        mActionBarDrawerToggle.syncState();
    }

    @Override
    public void onConfigurationChanged(Configuration newConfig){
        super.onConfigurationChanged(newConfig);
        mActionBarDrawerToggle.onConfigurationChanged(newConfig);
    }

//    private void loadImageInImageProcessor(String path)
//    {
//      if (path != null) {
//        //isProcessing flag -> then the function will be called on create
//        setFlags(true);
//
//        // Call Native c++  ------------------------------------------------------------------------------------------
//
//        Mat image_ = Imgcodecs.imread(path, Imgcodecs.CV_LOAD_IMAGE_COLOR);
//
//        String _buf = "Path: " + path + "\nResults: \n";
//        Vector result_list_ = getList(image_.getNativeObjAddr());
//
//        for (int i = 0; i < result_list_.size(); i++) {
//          _buf = _buf + result_list_.get(i).toString() + ", \n";
//        }
//
//        // Call Native c++  ------------------------------------------------------------------------------------------
//
//        AlertDialog.Builder builder_ = new AlertDialog.Builder(MainPage.this);
//
//        builder_.setMessage(_buf);
//        builder_.setCancelable(true);
//        builder_.setPositiveButton("OK",
//
//          new DialogInterface.OnClickListener() {
//            public void onClick(DialogInterface dialog, int id) {
//              dialog.cancel();
//              setFlags(false);
//            }
//          });
//
//        AlertDialog alert2 = builder_.create();
//        alert2.show();
//        image_path_ = null;
//      }
//    }

    public void setFlags(boolean flag_) {
      is_processing_.triggerBooleanListener(flag_);
    }

public void threadFinished(String _buf)
{
  AlertDialog.Builder builder_ = new AlertDialog.Builder(MainPage.this);

  builder_.setMessage(_buf);
  builder_.setCancelable(true);
  builder_.setPositiveButton("OK",

    new DialogInterface.OnClickListener() {
      public void onClick(DialogInterface dialog, int id)
      {
        dialog.cancel();
        setFlags(false);
      }
    });

  AlertDialog alert2 = builder_.create();
  alert2.show();
}


// functions to read files from assets and write them in storage

    private void copyAssets() {
        AssetManager assetManager = getAssets();
        String[] files = null;
        try {
            files = assetManager.list("datasets");
        } catch (IOException e) {
            Log.e("tag", "Failed to get asset file list.", e);
        }

        String folder_main = "datasets";

        File f = new File(Environment.getExternalStorageDirectory(), folder_main);
        if (!f.exists()) {
            f.mkdirs();


            for(String filename : files) {
                InputStream in = null;
                OutputStream out = null;
                try {
                    in = assetManager.open("datasets/" + filename);
                    File outFile = new File(f +"/", filename);
                    out = new FileOutputStream(outFile);
                    copyFile(in, out);
                    in.close();
                    in = null;
                    out.flush();
                    out.close();
                    out = null;
                } catch(IOException e) {
                    Log.e("tag", "Failed to copy asset file: " + filename, e);
                }
            }
        }


    }

    private void copyFile(InputStream in, OutputStream out) throws IOException {
        byte[] buffer = new byte[1024];
        int read;
        while((read = in.read(buffer)) != -1){
            out.write(buffer, 0, read);
        }
    }
  public static MainPage getActivity() {
    return main_instace_;
  }

//  //C++ function declaration
//  public native Vector getList(long matAddr);
}