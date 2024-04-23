package com.example.appdomotica;

import androidx.appcompat.app.AppCompatActivity;
import androidx.appcompat.widget.SwitchCompat;

import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.widget.CompoundButton;

import com.example.appdomotica.network.HttpRequestHandler;

public class SecurityActivity extends AppCompatActivity {

    private HttpRequestHandler httpRequestHandler = HttpRequestHandler.getInstance();
    SwitchCompat switch2;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_security);

        switch2 = (SwitchCompat) findViewById(R.id.switch2);
        switch2.setOnCheckedChangeListener(new CompoundButton.OnCheckedChangeListener() {
            @Override
            public void onCheckedChanged(CompoundButton buttonView, boolean isChecked) {
                String command;
                if(isChecked) {
                    command = "SEN";
                }
                else {
                    command = "SEF";
                }

                httpRequestHandler.sendCommand(command,  new HttpRequestHandler.ResponseCallback() {
                    @Override
                    public void onResponse(String response) {
                        // Handle the response here, e.g., update UI
                        Log.d("Response", response);
                    }
                });
            }
        });
        switch2.setOnClickListener();
    }

}