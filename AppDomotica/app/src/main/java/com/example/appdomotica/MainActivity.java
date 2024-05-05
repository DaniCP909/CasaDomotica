package com.example.appdomotica;

import androidx.appcompat.app.AppCompatActivity;

import android.content.Intent;
import android.util.Log;
import android.widget.Button;
import android.content.Context;
import android.net.ConnectivityManager;
import android.net.Network;
import android.net.NetworkCapabilities;
import android.net.NetworkRequest;
import android.os.Bundle;
import android.view.View;


import com.example.appdomotica.network.HttpRequestHandler;
import com.google.android.material.floatingactionbutton.FloatingActionButton;


public class MainActivity extends AppCompatActivity {
    private HttpRequestHandler httpRequestHandler = HttpRequestHandler.getInstance();
    Button buttonLightning;
    Button buttonHeating;
    Button buttonBlinds;
    Button buttonAirConditioning;
    Button buttonSecurity;
    FloatingActionButton buttonReset;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

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



        buttonLightning = findViewById(R.id.buttonLightning);
        buttonHeating = findViewById(R.id.buttonHeating);
        buttonBlinds = findViewById(R.id.buttonBlinds);
        buttonAirConditioning = findViewById(R.id.buttonAirConditioning);
        buttonSecurity = findViewById(R.id.buttonSecurity);
        buttonReset = findViewById(R.id.buttonReset);

        buttonLightning.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                Intent lightningActivity = new Intent(v.getContext(), LightningActivity.class);
                startActivity(lightningActivity);
            }
        });

        buttonHeating.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                Intent heatingActivity = new Intent(v.getContext(), HeatingActivity.class);
                startActivity(heatingActivity);
            }
        });

        buttonBlinds.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                Intent blindsActivity = new Intent(v.getContext(), BlindsActivity.class);
                startActivity(blindsActivity);
            }
        });

        buttonAirConditioning.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                Intent airConditioningActivity = new Intent(v.getContext(), AirConditioningActivity.class);
                startActivity(airConditioningActivity);
            }
        });

        buttonSecurity.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                Intent securityActivity = new Intent(v.getContext(), SecurityActivity.class);
                startActivity(securityActivity);
            }
        });

        buttonReset.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                String command = "RE";

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