package com.example.appdomotica;

import androidx.appcompat.app.AppCompatActivity;

import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.widget.Button;

import com.example.appdomotica.network.HttpRequestHandler;

public class HeatingActivity extends AppCompatActivity {

    private HttpRequestHandler httpRequestHandler = new HttpRequestHandler();

    Button buttonHeating;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_heating);
        buttonHeating = findViewById(R.id.buttonHeating);

        buttonHeating.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                String command = "CA";

                //if(ints_on) command += "off";
                //else command += "on";

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