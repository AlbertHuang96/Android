package com.example.renderplayground;

import static android.opengl.GLSurfaceView.RENDERMODE_CONTINUOUSLY;

import androidx.annotation.NonNull;
import androidx.appcompat.app.AppCompatActivity;
import androidx.core.app.ActivityCompat;

import android.Manifest;
import android.content.pm.PackageManager;
import android.hardware.Sensor;
import android.hardware.SensorEvent;
import android.hardware.SensorEventListener;
import android.hardware.SensorManager;
import android.os.Bundle;
import android.os.Environment;
import android.util.Log;
import android.view.ViewGroup;
import android.widget.RelativeLayout;

public class MainActivity extends AppCompatActivity implements SensorEventListener {

    private static final String[] REQUEST_PERMISSIONS = {
            Manifest.permission.WRITE_EXTERNAL_STORAGE,
            Manifest.permission.RECORD_AUDIO,
    };
    private static final int PERMISSION_REQUEST_CODE = 1;

    //private SensorManager mSensorManager;

    //private SurfaceViewGL mGLSurfaceView;
    //private RenderGL mGLRender = new RenderGL();

    public native int bindThreadToCore(int core);

    public native int bindPidToCore(int pid, int core);

    public native int bindThreadToCore2(int core);

    public native String getCpuInfo();


    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        //native_Helloworld();

        String cpuinfoCache = getCpuInfo();
        Log.e("Cpu cache : ", cpuinfoCache);
        //CPU Cache: L1 Data
        //     > size            : 32768
        //     > associativity   : 4
        //     > sets            : 128
        //     > partitions      : 1
        //     > line_size       : 64
        //     > flags           : 0
        //     > processor_start : 2
        //     > processor_count : 1
        //

        //
        //task1();
        //task2();

        //mGLRender.init();
        //mGLSurfaceView = new SurfaceViewGL(this, mGLRender);
        //mGLSurfaceView.setRenderMode(RENDERMODE_CONTINUOUSLY);
        //setContentView(mGLSurfaceView);
        //mSensorManager = (SensorManager) getSystemService(SENSOR_SERVICE);
    }

    private void task1() {
        new Thread(() -> {
            try {
                //bindThreadToCore(0);
                bindThreadToCore2(0);
                long time = System.currentTimeMillis();
                long sum = 0L;
                for (int i = 0; i < 1000000000L; i++) {
                    sum += i;
                }
                time = System.currentTimeMillis() - time;
                Log.e("task1 ", String.valueOf(time));
                ///com.example.renderplayground E/task1: 4716
                //com.example.renderplayground E/task1: 57
            } catch (Exception e) {
                e.printStackTrace();
            }
        }).start();
    }

    private void task2() {
        new Thread(() -> {
            try {
                //bindThreadToCore(1);
                bindThreadToCore2(1);
                long time = System.currentTimeMillis();
                long sum = 0L;
                for (int i = 0; i < 1000000000L; i++) {
                    sum += i;
                }
                time = System.currentTimeMillis() - time;
                Log.e("task2 ", String.valueOf(time));
                ///com.example.renderplayground E/task2: 4696
                //com.example.renderplayground E/task2: 62
            } catch (Exception e) {
                e.printStackTrace();
            }
        }).start();
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
        CommonUtils.copyAssetsDirToSDCard(MainActivity.this, "backpack", fileDir + "/model");
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
    static {
        System.loadLibrary("RangerEngine");
    }

    //public native String native_Helloworld();
}