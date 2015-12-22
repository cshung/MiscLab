package com.andrew.question;

import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;

public class InitialActivity extends AppCompatActivity {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_initial);
        // This is the key to remove the unwanted title bar - thanks StackOverflow
        // http://stackoverflow.com/questions/2868047/fullscreen-activity-in-android
        getSupportActionBar().hide();
    }
}