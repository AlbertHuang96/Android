package com.example.renderplayground;

import android.content.Context;

import java.io.File;
import java.lang.reflect.Array;
import java.lang.reflect.Field;
import java.util.HashSet;

import dalvik.system.DexClassLoader;
import dalvik.system.PathClassLoader;

public class FixDexJava {
    private static final String DEX_SUFFIX = ".dex";
    private static final String APK_SUFFIX = ".apk";
    private static final String JAR_SUFFIX = ".jar";
    private static final String ZIP_SUFFIX = ".zip";
    private static final String OPTIMIZE_DEX_DIR = "optimize_dex";

    private static HashSet<File> loadedDex = new HashSet<>();

    static {
        loadedDex.clear();
    }

    public FixDexJava() {

    }

    public void fixDex(Context context, String fileDirOdex) {
        if (prepareToFix(fileDirOdex)) {
            insertDexFile(context);
        }
    }

    public boolean prepareToFix(String fileDirOdex) {
        boolean canFix = false;
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
                        ((file.getName().endsWith(DEX_SUFFIX))
                        || file.getName().endsWith(APK_SUFFIX)
                        || file.getName().endsWith(JAR_SUFFIX)
                        || file.getName().endsWith(ZIP_SUFFIX))) {
                    loadedDex.add(file);
                    canFix = true;
                }
            }
        }
        return canFix;
    }

    private static void insertDexFile(Context context) {
        String optimizeDir = context.getFilesDir().getAbsolutePath() + File.separator + OPTIMIZE_DEX_DIR;
        File optFile = new File(optimizeDir);
        if (!optFile.exists()) {
            optFile.mkdirs();
        }

        //BaseDexClassLoader: DexPathList pathList
        // DexPathList: Element[] dexElements
        try {
            PathClassLoader pathLoader = (PathClassLoader) context.getClassLoader();
            for (File dex : loadedDex) {
                DexClassLoader dexLoader = new DexClassLoader(dex.getAbsolutePath(),
                        optFile.getAbsolutePath(), null, pathLoader);
                Object dexPathList = getPathList(dexLoader);
                Object pathPathList = getPathList(pathLoader);

                Object leftDexElements = getDexElements(dexPathList);
                Object rightDexElements = getDexElements(pathPathList);
                Object dexElements = combineArray(leftDexElements, rightDexElements);

                Object pathList = getPathList(pathLoader);
                setField(pathList, pathList.getClass(), "dexElements", dexElements);
            }
        } catch (Exception e) {
            e.printStackTrace();
        }
    }

    private static void setField(Object obj, Class<?> cl, String field, Object value) throws NoSuchFieldException, IllegalAccessException {
        Field declaredField = cl.getDeclaredField(field);
        declaredField.setAccessible(true);
        declaredField.set(obj, value);
    }

    private static Object getField(Object obj, Class<?> cl, String field) throws NoSuchFieldException, IllegalAccessException {
        Field localField = cl.getDeclaredField(field);
        localField.setAccessible(true);
        return localField.get(obj);
    }

    private static Object getPathList(Object baseDexClassloader) throws ClassNotFoundException, NoSuchFieldException, IllegalAccessException {
        return getField(baseDexClassloader, Class.forName("dalvik.system.BaseDexClassLoader"), "pathList");
    }

    private static Object getDexElements(Object pathList) throws NoSuchFieldException, IllegalAccessException {
        return getField(pathList, pathList.getClass(), "dexElements");
    }

    private static Object combineArray(Object arrayLHS, Object arrayRHS) {
        Class<?> clazz = arrayLHS.getClass().getComponentType();
        int i = Array.getLength(arrayLHS);
        int j = Array.getLength(arrayRHS);
        int k = i + j;
        Object result = Array.newInstance(clazz, k);
        System.arraycopy(arrayLHS, 0, result, 0, i);
        System.arraycopy(arrayRHS, 0, result, i, j);
        return result;
    }
}
