package com.example.injectclassplugin

import org.gradle.api.Project
import javassist.*

class InjectClass {
    private final static ClassPool pool = ClassPool.getDefault();
    static void inject(String path, Project project, String injectCode) {
        println("filePath = "+path)
        pool.appendClassPath(path)
        pool.appendClassPath(project.android.bootClasspath[0].toString())

        File dir = new File(path)
        if (dir.isDirectory()) {
            dir.eachFileRecurse {File file ->
                if (file.getName().equals("MainActivity.class")) {
                    CtClass ctClass = pool.getCtClass("com.example.renderplayground.MainActivity")
                    println("ctClass = " + ctClass)
                    if (ctClass.isFrozen()) ctClass.defrost()
                    CtMethod ctMethod = ctClass.getDeclaredMethod("onCreate")
                    println("Method name = " + ctMethod)
                    println("injectCode = " + injectCode)
                    //ctMethod.insertBefore(injectCode)
                    ctMethod.insertAfter(injectCode)
                    ctClass.writeFile(path)
                    ctClass.detach()
                }
            }
        }
    }
}