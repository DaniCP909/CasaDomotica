package com.example.appdomotica;

import androidx.appcompat.app.AppCompatActivity;

import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.widget.Button;

import com.example.appdomotica.network.HttpRequestHandler;

public class AirConditioningActivity extends AppCompatActivity {

    private HttpRequestHandler httpRequestHandler = HttpRequestHandler.getInstance();

    Button buttonAirConditioning;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_air_conditioning);
        buttonAirConditioning = findViewById(R.id.buttonAirConditioning);

        buttonAirConditioning.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                String command = "VE";

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