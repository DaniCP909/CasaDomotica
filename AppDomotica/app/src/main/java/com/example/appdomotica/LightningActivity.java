package com.example.appdomotica;

import androidx.appcompat.app.AppCompatActivity;
import androidx.appcompat.widget.SwitchCompat;

import android.content.Context;
import android.net.ConnectivityManager;
import android.net.Network;
import android.net.NetworkCapabilities;
import android.net.NetworkRequest;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.widget.Button;
import android.widget.CompoundButton;
import android.widget.Switch;

import com.example.appdomotica.network.HttpRequestHandler;

public class LightningActivity extends AppCompatActivity {

    private HttpRequestHandler httpRequestHandler = HttpRequestHandler.getInstance();

    //private Boolean ints_on;
    //private Boolean exts_on;

    Button buttonIntLight;
    Button buttonExtLight;
    SwitchCompat switchIntLight;
    SwitchCompat switchExtLight;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_lightning);

        ConnectivityManager connectivityManager = (ConnectivityManager) getSystemService(Context.CONNECTIVITY_SERVICE);
        NetworkRequest.Builder builder = new NetworkRequest.Builder();
        builder.addTransportType(NetworkCapabilities.TRANSPORT_WIFI);
        builder.addCapability(NetworkCapabilities.NET_CAPABILITY_INTERNET);

        NetworkRequest request = builder.build();
        connectivityManager.requestNetwork(request, new ConnectivityManager.NetworkCallback() {
            @Override
            public void onAvailable(Network network) {
                connectivityManager.bindProcessToNetwork(network);
            }
        });

        //ints_on = false;
        //exts_on = false;

        buttonIntLight = findViewById(R.id.buttonIntLight);
        buttonExtLight = findViewById(R.id.buttonExtLight);
        switchIntLight = (SwitchCompat) findViewById(R.id.switchIntLight);
        switchExtLight = (SwitchCompat) findViewById(R.id.switchExtLight);

        buttonIntLight.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                String command = "LI";

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


        buttonExtLight.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                String command = "LE";

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

        switchIntLight.setOnCheckedChangeListener(new CompoundButton.OnCheckedChangeListener() {
            @Override
            public void onCheckedChanged(CompoundButton buttonView, boolean isChecked) {
                String command;
                if(isChecked) {
                    command = "LIAN";
                }
                else {
                    command = "LIAF";
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
        switchExtLight.setOnCheckedChangeListener(new CompoundButton.OnCheckedChangeListener() {
            @Override
            public void onCheckedChanged(CompoundButton buttonView, boolean isChecked) {
                String command;
                if(isChecked) {
                    command = "LEAN";
                }
                else {
                    command = "LEAF";
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
    }
}