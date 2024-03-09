package com.example.renderplayground;

import static android.opengl.GLSurfaceView.RENDERMODE_CONTINUOUSLY;

import androidx.annotation.NonNull;
import androidx.appcompat.app.AppCompatActivity;
import androidx.core.app.ActivityCompat;

import android.Manifest;
import android.content.ComponentName;
import android.content.Context;
import android.content.Intent;
import android.content.ServiceConnection;
import android.content.pm.PackageManager;
import android.hardware.Sensor;
import android.hardware.SensorEvent;
import android.hardware.SensorEventListener;
import android.hardware.SensorManager;
import android.os.Bundle;
import android.os.Environment;
import android.os.IBinder;
import android.os.Parcel;
import android.util.Log;
import android.view.ViewGroup;
import android.widget.RelativeLayout;

import java.io.BufferedReader;
import java.io.FileDescriptor;
import java.io.FileReader;

public class MainActivity extends AppCompatActivity implements SensorEventListener {

    private static final String[] REQUEST_PERMISSIONS = {
            Manifest.permission.WRITE_EXTERNAL_STORAGE,
            Manifest.permission.RECORD_AUDIO,
    };
    private static final int PERMISSION_REQUEST_CODE = 1;

    //private SensorManager mSensorManager;

    //private SurfaceViewGL mGLSurfaceView;
    //private RenderGL mGLRender = new RenderGL();

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        Intent intent = new Intent(this, RemoteService.class);
        bindService(intent, new ServiceConnection() {
            @Override
            public void onServiceConnected(ComponentName name, IBinder service) {
                Parcel data = Parcel.obtain();
                Parcel reply = Parcel.obtain();
                try {
                    service.transact(1, data, reply, 0);
                    FileDescriptor fd = reply.readFileDescriptor().getFileDescriptor();
                    BufferedReader br = new BufferedReader(new FileReader(fd));
                    Log.v("huang ashmem", br.readLine());
                } catch (Exception e) {
                    e.printStackTrace();
                }

            }

            @Override
            public void onServiceDisconnected(ComponentName name) {

            }
        }, Context.BIND_AUTO_CREATE);
        //native_Helloworld();
        //mGLRender.init();
        //mGLSurfaceView = new SurfaceViewGL(this, mGLRender);
        //mGLSurfaceView.setRenderMode(RENDERMODE_CONTINUOUSLY);
        //setContentView(mGLSurfaceView);

        //mSensorManager = (SensorManager) getSystemService(SENSOR_SERVICE);
    }

    @Override
    protected void onResume() {
        super.onResume();
        //mSensorManager.registerListener(this, mSensorManager.getDefaultSensor(Sensor.TYPE_GRAVITY), SensorManager.SENSOR_DELAY_FASTEST);
        if (!hasPermissionsGranted(REQUEST_PERMISSIONS)) {
            ActivityCompat.requestPermissions(this, REQUEST_PERMISSIONS, PERMISSION_REQUEST_CODE);
        }
        ///sdcard/Android/data/com.byteflow.app/files/Download
        String fileDir = getExternalFilesDir(Environment.DIRECTORY_DOWNLOADS).getAbsolutePath();
        //CommonUtils.copyAssetsDirToSDCard(MainActivity.this, "poly", fileDir + "/model");
        //CommonUtils.copyAssetsDirToSDCard(MainActivity.this, "backpack", fileDir + "/model");
    }

    @Override
    protected void onPause() {
        super.onPause();
        //mSensorManager.unregisterListener(this);
    }

    @Override
    public void onRequestPermissionsResult(int requestCode, @NonNull String[] permissions, @NonNull int[] grantResults) {
        super.onRequestPermissionsResult(requestCode, permissions, grantResults);
    }

    @Override
    public void onSensorChanged(SensorEvent event) {

    }

    @Override
    public void onAccuracyChanged(Sensor sensor, int accuracy) {

    }

    protected boolean hasPermissionsGranted(String[] permissions) {
        for (String permission : permissions) {
            if (ActivityCompat.checkSelfPermission(this, permission)
                    != PackageManager.PERMISSION_GRANTED) {
                return false;
            }
        }
        return true;
    }

    /*static {
        System.loadLibrary("RangerEngine");
    }*/

    //public native String native_Helloworld();
}