package com.example.jimmyhalimi.snapmusic;

import android.content.Intent;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.provider.MediaStore;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.util.Log;
import android.widget.EditText;
import android.widget.ImageView;
import android.widget.Toast;

import java.io.IOException;

public class Image extends AppCompatActivity {

  ImageView ivImage;


  @Override
  protected void onCreate(Bundle savedInstanceState) {
    super.onCreate(savedInstanceState);
    setContentView(R.layout.activity_image);

    ivImage = (ImageView) findViewById(R.id.ivImage);


    if(getIntent().hasExtra("data1")) {
      Bundle bd = getIntent().getExtras();
      Intent data = bd.getParcelable("data1");
      Bitmap bm = null;
      try {
        bm = MediaStore.Images.Media.getBitmap(getApplicationContext().getContentResolver(), data.getData());
      } catch (IOException e) {
        e.printStackTrace();
      }
      String gallery_path = getIntent().getStringExtra("galleryPath");
      ivImage.setImageBitmap(bm);
      Toast.makeText(Image.this, "Path:  " + gallery_path, Toast.LENGTH_SHORT).show();
    }

    if(getIntent().hasExtra("path1")){


      String image_path_ = getIntent().getStringExtra("path1");
      Bitmap myBitmap = BitmapFactory.decodeFile(image_path_);
      ivImage.setImageBitmap(myBitmap);
      Toast.makeText(Image.this, "Path:  " + image_path_, Toast.LENGTH_SHORT).show();
    }






  }


}
