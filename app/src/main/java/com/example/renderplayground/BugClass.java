package com.example.renderplayground;


/*public class BugClass {
    public int test() {
        throw new RuntimeException("BugClass: this is a exception -- albert huang");
    }
}*/

//Caused by: java.lang.RuntimeException: BugClass: this is a exception -- albert huang
//        at com.example.renderplayground.BugClass.test(BugClass.java:6)
//        at com.example.renderplayground.MainActivity.onCreate(MainActivity.java:51)
//        at android.app.Activity.performCreate(Activity.java:8238)
//        at android.app.Activity.performCreate(Activity.java:8206)
//        at android.app.Instrumentation.callActivityOnCreate(Instrumentation.java:1329)
//        at android.app.ActivityThread.performLaunchActivity(ActivityThread.java:3695)

public class BugClass {
    @Replace(clazz = "com.example.renderplayground.BugClass", method = "test")
    public static int test() {
        //throw new RuntimeException("BugClass: this is a exception -- albert huang");
        return 1;
    }
}