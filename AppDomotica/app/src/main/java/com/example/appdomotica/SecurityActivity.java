package com.example.appdomotica;

import androidx.appcompat.app.AppCompatActivity;
import androidx.appcompat.widget.SwitchCompat;

import android.os.Bundle;

public class SecurityActivity extends AppCompatActivity {

    SwitchCompat switch2;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_security);

        switch2 = findViewById(R.id.switch2);
    }
}