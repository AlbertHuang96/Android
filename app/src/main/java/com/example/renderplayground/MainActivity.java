package com.example.renderplayground;

import static android.opengl.GLSurfaceView.RENDERMODE_CONTINUOUSLY;

import androidx.annotation.NonNull;
import androidx.appcompat.app.AppCompatActivity;
import androidx.core.app.ActivityCompat;

import android.Manifest;
import android.content.pm.ActivityInfo;
import android.content.pm.PackageManager;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.hardware.Sensor;
import android.hardware.SensorEvent;
import android.hardware.SensorEventListener;
import android.hardware.SensorManager;
import android.os.Bundle;
import android.os.Environment;
import android.util.Log;
import android.view.View;
import android.view.ViewGroup;
import android.widget.Button;
import android.widget.ImageView;
import android.widget.RelativeLayout;

import java.io.IOException;
import java.io.InputStream;
import java.nio.ByteBuffer;

public class MainActivity extends AppCompatActivity implements SensorEventListener {

    private static final String[] REQUEST_PERMISSIONS = {
            Manifest.permission.WRITE_EXTERNAL_STORAGE,
            Manifest.permission.RECORD_AUDIO,
    };
    private static final int PERMISSION_REQUEST_CODE = 1;

    public static final int IMAGE_FORMAT_RGBA = 0x01;

    private SensorManager mSensorManager;

    private SurfaceViewGL mGLSurfaceView;
    private RenderGL mGLRender = new RenderGL();

    ImageView ivSrc;
    ImageView ivDst;
    Button btnConvert;

    Bitmap srcImg, dstImg;
    float dstHeight = 100, dstWidth = 100;


    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        //setRequestedOrientation(ActivityInfo.SCREEN_ORIENTATION_FULL_SENSOR);
        //setContentView(R.layout.activity_main);
        //native_Helloworld();
        mGLRender.init();
        mGLSurfaceView = new SurfaceViewGL(this, mGLRender);
        mGLSurfaceView.setRenderMode(RENDERMODE_CONTINUOUSLY);
        //setContentView(mGLSurfaceView);
        mSensorManager = (SensorManager) getSystemService(SENSOR_SERVICE);

        setContentView(R.layout.activity_main);

        //Bitmap bitmap = loadRGBAImage(R.drawable.uv1024);
        //srcImg = BitmapFactory.decodeResource(getResources(), R.drawable.uv1024);

        btnConvert = findViewById(R.id.btnConvert);
        ivSrc = findViewById(R.id.ivSrc);
        ivDst = findViewById(R.id.ivDst);

        // app crash if loading directly OOM
        //ivSrc.setImageBitmap(bitmap);

        try {
            BitmapFactory.Options opts = new BitmapFactory.Options();
            opts.inJustDecodeBounds = true;
            srcImg = BitmapFactory.decodeResource(getResources(), R.drawable.uv1024, opts);
            float srcWidth = opts.outWidth;
            float srcHeight = opts.outHeight;
            int inSampleSize = 1;

            if (srcHeight > dstHeight || srcWidth > dstWidth) {
                if (srcWidth > srcHeight) {
                    inSampleSize = Math.round(srcHeight / dstHeight);
                } else {
                    inSampleSize = Math.round(srcWidth / dstWidth);
                }
            }

            inSampleSize = inSampleSize+1;

            opts.inJustDecodeBounds = false;
            opts.inSampleSize = inSampleSize;
            srcImg = BitmapFactory.decodeResource(getResources(), R.drawable.uv1024, opts);
            dstImg = srcImg.copy(Bitmap.Config.ARGB_8888, true);
            ivSrc.setImageBitmap(srcImg);

        } catch (Exception e) {
            e.printStackTrace();
        }

        btnConvert.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                try {
                    processImage(dstImg);
                    ivDst.setImageBitmap(dstImg);
                } catch (Exception e) {
                    e.printStackTrace();
                }
            }
        });

        //setContentView(R.layout.activity_main);
    }

    public native int processImage(Bitmap src);

    @Override
    protected void onResume() {
        super.onResume();
        mSensorManager.registerListener(this,
                mSensorManager.getDefaultSensor(Sensor.TYPE_GRAVITY),
                SensorManager.SENSOR_DELAY_FASTEST);
        if (!hasPermissionsGranted(REQUEST_PERMISSIONS)) {
            ActivityCompat.requestPermissions(this, REQUEST_PERMISSIONS, PERMISSION_REQUEST_CODE);
        }
        ///sdcard/Android/data/com.byteflow.app/files/Download
        String fileDir = getExternalFilesDir(Environment.DIRECTORY_DOWNLOADS).getAbsolutePath();
        CommonUtils.copyAssetsDirToSDCard(MainActivity.this, "poly", fileDir + "/model");
        CommonUtils.copyAssetsDirToSDCard(MainActivity.this, "backpack", fileDir + "/model");
    }

    @Override
    protected void onPause() {
        super.onPause();
        mSensorManager.unregisterListener(this);
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

    private Bitmap loadRGBAImage(int resId) {
        InputStream is = this.getResources().openRawResource(resId);
        Bitmap bitmap;
        try {
            bitmap = BitmapFactory.decodeStream(is);
            if (bitmap != null) {
                int bytes = bitmap.getByteCount();
                ByteBuffer buf = ByteBuffer.allocate(bytes);
                bitmap.copyPixelsToBuffer(buf);
                byte[] byteArray = buf.array();
                mGLRender.setImageData(IMAGE_FORMAT_RGBA, bitmap.getWidth(), bitmap.getHeight(), byteArray);
            }
        }
        finally
        {
            try
            {
                is.close();
            }
            catch(IOException e)
            {
                e.printStackTrace();
            }
        }
        return bitmap;
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