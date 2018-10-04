package com.example.jimmyhalimi.snapmusic;

import android.content.Intent;
import android.os.Bundle;

import android.support.v7.app.AppCompatActivity;


public class MainPage extends AppCompatActivity {


  @Override
  protected void onCreate(Bundle savedInstanceState) {
    super.onCreate(savedInstanceState);
    setContentView(R.layout.main_page);

    Thread th = new Thread()
    {
      public void run()
      {
        try {
          sleep(2000);
        }
        catch (Exception e)
        {
          e.printStackTrace();
        }
        finally {
          Intent objI = new Intent(MainPage.this, Home.class);
          startActivity(objI);
        }
      }
    };

    th.start();

  }

    @Override
    protected void onPause() {
        super.onPause();
        finish();
    }

}