package com.example.renderplayground;

import android.app.Service;
import android.content.Intent;
import android.os.Binder;
import android.os.IBinder;
import android.os.MemoryFile;
import android.os.Parcel;
import android.os.ParcelFileDescriptor;
import android.os.RemoteException;

import java.io.FileDescriptor;
import java.lang.reflect.Method;

public class RemoteService extends Service {
    public RemoteService() {
    }

    @Override
    public IBinder onBind(Intent intent) {
        // TODO: Return the communication channel to the service.
        //throw new UnsupportedOperationException("Not yet implemented");
        return new MyBinder();
    }

    public class MyBinder extends Binder {
        @Override
        protected boolean onTransact(int code, Parcel data, Parcel reply, int flags) throws RemoteException {
            if (code == 1)
            {
                try {
                    String str = "AlbertHuang";
                    byte[] contents = str.getBytes();
                    MemoryFile mf = new MemoryFile("memfile", contents.length);
                    mf.writeBytes(contents, 0, 0, contents.length);

                    Method method = MemoryFile.class.getDeclaredMethod("getFileDescriptor");
                    FileDescriptor fd = (FileDescriptor) method.invoke(mf);
                    ParcelFileDescriptor pfd = ParcelFileDescriptor.dup(fd);
                    reply.writeFileDescriptor(fd);
                    return true;
                } catch (Exception e) {
                    e.printStackTrace();
                }
            }
            return super.onTransact(code, data, reply, flags);
        }
    }
}