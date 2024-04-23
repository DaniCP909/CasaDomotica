package com.example.appdomotica.network;

import android.util.Log;

import java.io.IOException;

import okhttp3.OkHttpClient;
import okhttp3.Request;
import okhttp3.Response;

public class HttpRequestHandler {

    private OkHttpClient client = new OkHttpClient();
    private static HttpRequestHandler handler;

    private HttpRequestHandler(){}

    public static HttpRequestHandler getInstance(){
        if(handler == null){
            handler = new HttpRequestHandler();
        }
        return handler;
    }

    public interface ResponseCallback {
        void onResponse(String response);
    }

    public void sendCommand(String cmd, final ResponseCallback callback) {
        new Thread(new Runnable() {
            @Override
            public void run() {
                String command = "http://192.168.4.1/" + cmd;
                Log.d("Command-----------------------------------", cmd);
                Request request = new Request.Builder().url(command).build();
                try {
                    Response response = client.newCall(request).execute();
                    String myPrintableResponse = response.body().string();
                    String cleanResponse = myPrintableResponse.replaceAll("\\<.*?\\>", "")
                            .replace("\n", "")
                            .replace("\r", "")
                            .replace(" ", "")
                            .replace("\t", "")
                            .trim();
                    Log.d("Response = ", cleanResponse);

                    // Call the callback on the main thread
                    if (callback != null) {
                        callback.onResponse(cleanResponse);
                    }

                } catch (IOException e) {
                    Log.e("HttpRequestHandler", "Error executing request", e);
                }
            }
        }).start();
    }
}
