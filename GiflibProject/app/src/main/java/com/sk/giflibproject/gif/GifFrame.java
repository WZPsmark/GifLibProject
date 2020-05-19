package com.sk.giflibproject.gif;

import android.content.Context;
import android.content.res.AssetManager;
import android.graphics.Bitmap;

import java.io.InputStream;

/**
 * Created by smark on 2020/5/19.
 * 邮箱：smarkwzp@163.com
 */
public class GifFrame {

    static {
        System.loadLibrary("native-lib");
    }

    private int width;
    private int height;
    private int frameCount;

    private long nativeHandle;//C++ 一帧的对象 指针


    public GifFrame(long nativeHandle,int width,int height,int frameCount){
        this.nativeHandle = nativeHandle;
        this.width = width;
        this.height = height;
        this.frameCount = frameCount;
    }

    public static GifFrame decodeStream(InputStream stream){
        byte[] buffer = new byte[16 * 1024];
        return nativeDecodeStream(stream,buffer);
    }

    public static GifFrame decodeStream(final Context context, final String fileName){
        return nativeDecodeStreamJNI((context == null)? null:context.getAssets(),fileName);
    }


    public int getWidth() {
        return width;
    }

    public void setWidth(int width) {
        this.width = width;
    }

    public int getHeight() {
        return height;
    }

    public void setHeight(int height) {
        this.height = height;
    }

    public int getFrameCount() {
        return frameCount;
    }


    public long getFrame(Bitmap bitmap, int frameIndex) {
        return nativeGetFrame(nativeHandle,bitmap,frameIndex);
    }


    /**
     * java层自己加载后将文件流传进去，使用不同文件路径及网络加载
     * @param stream
     * @param buffer
     * @return
     */
    private static native GifFrame nativeDecodeStream(InputStream stream, byte[] buffer);

    /**
     * 使用C++去加载asset中的gif文件
     * @param assetManager
     * @param gifPath
     * @return
     */
    private static native GifFrame nativeDecodeStreamJNI(AssetManager assetManager, String gifPath);


    /**
     * 获取某一帧
     * @param nativeHandle
     * @param bitmap
     * @param frameIndex
     * @return
     */
    private static native long nativeGetFrame(long nativeHandle, Bitmap bitmap, int frameIndex);


}
