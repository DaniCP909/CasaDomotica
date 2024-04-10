package com.example.appdomotica;

import androidx.appcompat.app.AppCompatActivity;

import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.widget.Button;

import com.example.appdomotica.network.HttpRequestHandler;

public class LightningActivity extends AppCompatActivity {

    private HttpRequestHandler httpRequestHandler = new HttpRequestHandler();

    private Boolean ints_on;
    private Boolean exts_on;

    Button buttonIntLight;
    Button buttonExtLight;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_lightning);

        ints_on = false;
        exts_on = false;

        buttonIntLight = findViewById(R.id.buttonIntLight);
        buttonExtLight = findViewById(R.id.buttonExtLight);

        buttonIntLight.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                String command = "int_light_";

                if(ints_on) command += "off";
                else command += "on";

                httpRequestHandler.sendCommand(command,  new HttpRequestHandler.ResponseCallback() {
                    @Override
                    public void onResponse(String response) {
                        // Handle the response here, e.g., update UI
                        Log.d("Response", response);
                    }
                });
            }
        });

        buttonIntLight.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                String command = "ext_light_";

                if(ints_on) command += "off";
                else command += "on";

                httpRequestHandler.sendCommand(command,  new HttpRequestHandler.ResponseCallback() {
                    @Override
                    public void onResponse(String response) {
                        // Handle the response here, e.g., update UI
                        Log.d("Response", response);
                    }
                });
            }
        });
    }
}