package com.example.jimmyhalimi.snapmusic;

import android.Manifest;
import android.content.pm.PackageManager;
import android.support.v4.app.ActivityCompat;
import android.support.v4.content.ContextCompat;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.util.Log;
import android.widget.CompoundButton;
import android.widget.Switch;

public class Settings extends AppCompatActivity {

    Switch swCamera, swStorage;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_settings);

        swCamera = (Switch)findViewById(R.id.swCamera);
        swStorage = (Switch)findViewById(R.id.swStorage);



    }



}
