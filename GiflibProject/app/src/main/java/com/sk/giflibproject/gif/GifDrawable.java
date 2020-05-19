package com.sk.giflibproject.gif;

import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.graphics.Canvas;
import android.graphics.ColorFilter;
import android.graphics.Paint;
import android.graphics.PixelFormat;
import android.graphics.Rect;
import android.graphics.drawable.Animatable;
import android.graphics.drawable.Drawable;
import android.os.SystemClock;

import androidx.annotation.NonNull;
import androidx.annotation.Nullable;

/**
 * Created by smark on 2020/5/19.
 * 邮箱：smarkwzp@163.com
 */
public class GifDrawable extends Drawable implements Animatable, Runnable {

    private Paint mPaint;

    private Rect srcRect;//Gif的矩形区域

    private Bitmap mBitmap;

    private GifFrame mGifFrame;

    private int width;//gif图的宽

    private int height;//gif图的高

    private boolean isRunning;

    private long showTime;
    private long curTime;//当前帧需要显示的时间

    private int frameCount;//需要显示的帧数
    private int frameIndex;//当前显示的帧数

    public GifDrawable(GifFrame gifFrame) {
        mGifFrame = gifFrame;
        width = mGifFrame.getWidth();
        height = mGifFrame.getHeight();
        frameCount = mGifFrame.getFrameCount();
        mBitmap = Bitmap.createBitmap(width, height, Bitmap.Config.ARGB_8888);

        mPaint = new Paint();
        mPaint.setFilterBitmap(true);
        frameIndex = 0;
        srcRect = new Rect(0, 0, width, height);
        mGifFrame.getFrame(mBitmap, getFrameIndex());

    }


    public int getFrameIndex() {
        frameIndex++;
        return frameIndex < frameCount ? frameIndex : (frameIndex = 0);
    }

    @Override
    public void start() {
        if (!isRunning()) {
            isRunning = true;
            scheduleSelf(this, SystemClock.uptimeMillis());
        }

    }

    @Override
    public void stop() {
        if (isRunning()) {
            isRunning = false;
            unscheduleSelf(this);
        }

    }

    @Override
    public boolean isRunning() {
        return isRunning;
    }

    @Override
    public void draw(@NonNull Canvas canvas) {
        canvas.drawBitmap(mBitmap, srcRect, getBounds(), mPaint);
        showTime = SystemClock.uptimeMillis();
        if (isRunning()) {
            scheduleSelf(this, showTime + curTime);
        }

    }

    @Override
    public void setAlpha(int alpha) {
        mPaint.setAlpha(alpha);
    }

    @Override
    public void setColorFilter(@Nullable ColorFilter colorFilter) {
        mPaint.setColorFilter(colorFilter);
    }

    @Override
    public int getOpacity() {
        return PixelFormat.TRANSLUCENT;
    }

    @Override
    public void run() {

        curTime = mGifFrame.getFrame(mBitmap, getFrameIndex());
        invalidateSelf();

    }
}
