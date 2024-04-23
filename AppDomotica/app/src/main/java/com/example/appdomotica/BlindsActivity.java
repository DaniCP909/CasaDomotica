package com.example.appdomotica;

import androidx.appcompat.app.AppCompatActivity;

import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.widget.Button;

import com.example.appdomotica.network.HttpRequestHandler;

public class BlindsActivity extends AppCompatActivity {

    private HttpRequestHandler httpRequestHandler = HttpRequestHandler.getInstance();

    Button buttonBlinds;
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_blinds);
        buttonBlinds = findViewById(R.id.buttonBlinds);

        buttonBlinds.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                String command = "PE";

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