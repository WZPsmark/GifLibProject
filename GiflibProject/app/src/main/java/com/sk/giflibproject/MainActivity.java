package com.sk.giflibproject;

import android.Manifest;
import android.content.res.AssetManager;
import android.os.Build;
import android.os.Bundle;
import android.widget.ImageView;

import androidx.appcompat.app.AppCompatActivity;

import com.sk.giflibproject.gif.GifDrawable;
import com.sk.giflibproject.gif.GifFrame;

import java.io.IOException;

public class MainActivity extends AppCompatActivity {

    ImageView tv;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.M) {
            requestPermissions(new String[]{Manifest.permission.WRITE_EXTERNAL_STORAGE, Manifest.permission.READ_EXTERNAL_STORAGE}, 666);
        }

        // Example of a call to a native method
         tv = findViewById(R.id.image);
        GifDrawable gifDrawable = null;

        try {
            gifDrawable =new GifDrawable(GifFrame.decodeStream(getAssets().open("fire.gif")));
//            gifDrawable = new GifDrawable(GifFrame.decodeStream(this,"time_1.gif"));
//            gifDrawable = new GifDrawable(GifFrame.decodeStream(null,"/sdcard/timg.gif"));
            tv.setImageDrawable(gifDrawable);
            gifDrawable.start();
        } catch (Exception e) {
            e.printStackTrace();
        }

    }


}
