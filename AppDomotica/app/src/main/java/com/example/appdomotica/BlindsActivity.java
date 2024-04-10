package com.example.appdomotica;

import androidx.appcompat.app.AppCompatActivity;

import android.os.Bundle;

import com.example.appdomotica.network.HttpRequestHandler;

public class BlindsActivity extends AppCompatActivity {

    private HttpRequestHandler httpRequestHandler = new HttpRequestHandler();

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_blinds);
    }
}