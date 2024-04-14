package com.example.renderplayground;

import android.content.Context;
import android.os.Environment;

import java.io.File;
import java.lang.reflect.Method;

import dalvik.system.DexClassLoader;
//import dalvik.system.DexFile;

public class FixDexNative {
    private static final String DEX_SUFFIX = ".dex";
    private static final String APK_SUFFIX = ".apk";
    private static final String JAR_SUFFIX = ".jar";
    private static final String ZIP_SUFFIX = ".zip";

    public static final String DEX_DIR = "odex";

    private Context context;

    public FixDexNative(Context context) {

        this.context = context;
    }

    public void prepareToFix(String fileDirOdex) {
        File externalStorageDirectory = Environment.getExternalStorageDirectory();
        System.out.println("externalStorageDirectory name "+externalStorageDirectory.getName());

        System.out.println("context.getFilesDir() name "+context.getFilesDir());
        //File fileDir = externalStorageDirectory != null ?
        //        new File(externalStorageDirectory, "23333") :
        //        new File(context.getFilesDir(), DEX_DIR);
        // data/data/pkg's name/files/odex

        //context.getFilesDir()
        //No additional permissions are required for the calling app to read or write files under the returned path.

        //File fileDir = new File(context.getFilesDir(), DEX_DIR);
        //System.out.println("fileDirEXT name "+fileDirEXT);
        //File fileDir = new File(fileDirEXT, DEX_DIR);
        //File fileDir = new File(externalStorageDirectory, "23333");

        File fileDir = new File(fileDirOdex, "hotfix");
        if (!fileDir.exists()) {
            System.out.println("fileDir not exists ");
            //fileDir.mkdirs();
        }

        File[] listFiles = fileDir.listFiles();
        if (listFiles != null) {
            for (File file : listFiles) {
                System.out.println("file name "+file.getName());
                if (file.getName().startsWith("fix") &&
                        (file.getName().endsWith(DEX_SUFFIX))) {
                    loadDex(file);
                }
            }
        }
    }

    public void loadDex(File file) {
        //DexFile dalvik.system.DexFile already deprecated
        try {
            //DexClassLoader
            DexClassLoader classLoader = new DexClassLoader(file.getAbsolutePath(), context.getFilesDir().getAbsolutePath(), null, context.getClassLoader());
            Class realClazz = classLoader.loadClass("com.example.renderplayground.BugClass");
            // need to handle ClassNotFoundException
            if (realClazz != null) {
                fixClazz(realClazz);
            }
        } catch (Exception e) {
            e.printStackTrace();
        }
    }

    private void fixClazz(Class realClazz) {
        Method[] methods = realClazz.getMethods();
        for (Method rightMethod : methods) {
            Replace replace = rightMethod.getAnnotation(Replace.class);
            if (replace == null) {
                continue;
            }

            String clazzName = replace.clazz();
            String methodName = replace.method();
            try {
                Class wrongClazz = Class.forName(clazzName);
                Method wrongMethod = wrongClazz.getDeclaredMethod(methodName, rightMethod.getParameterTypes());
                replace(wrongMethod, rightMethod, NativeStructModel.class);
            } catch (IllegalArgumentException e2) {
                System.out.println("fixClazz IllegalArgumentException ");
                e2.printStackTrace();
            } catch (Exception e) {
                e.printStackTrace();
            }
        }
    }

    public native static void replace(Method wrongMethod, Method rightMethod, Class clazz2);
    // https://developer.aliyun.com/article/74598#
    // https://cloud.tencent.com/developer/article/1633531
}


