//
// Created by DIY on 2020/5/19.
//

#include "GifFrame.h"
#include <android/bitmap.h>
#include <android/asset_manager.h>
#include <android/asset_manager_jni.h>
#include "log.h"


#define ARGB_TO_COLOR8888(a, r, g, b) \
((a) << 24 | (b) << 16 | (g) << 8 | (r))


static int readFromByInputStream(GifFileType *fileType, GifByteType *out, int size) {
    JavaInputStream *stream = static_cast<JavaInputStream *>(fileType->UserData);
    return stream->read(out, (size_t)size);
}

static int readFromByAAsset(GifFileType *fileType, GifByteType *out, int size) {
    AAsset *aAsset = static_cast<AAsset *>(fileType->UserData);
    return AAsset_read(aAsset, out, (size_t)size);

}

static long getDelayMs(GraphicsControlBlock &gcb) {
    return gcb.DelayTime * 10;
}

GifFrame::GifFrame(JavaInputStream *stream) {
    ///打开gif图片的文件，通过这个readFromByInputStream回调获取gif文件的内容
    mGif = DGifOpen(stream, readFromByInputStream, NULL);
    ///初始化结构图
    DGifSlurp(mGif);
    GraphicsControlBlock gcb;
    long durationMs = 0;
    for (int i = 0; i < mGif->ImageCount; i++) {
        ///获取显示时间
        DGifSavedExtensionToGCB(mGif, i, &gcb);
        durationMs += getDelayMs(gcb);
    }


}

GifFrame::GifFrame(JNIEnv *env, jobject assetManager, const char *fileName) {

    LOGI("assetManager:%p", assetManager);
    LOGI("fileName:%p", fileName);
    if (assetManager) {
        AAssetManager *mgr = AAssetManager_fromJava(env, assetManager);
        AAsset *aAsset = AAssetManager_open(mgr, fileName, AASSET_MODE_STREAMING);
        mGif = DGifOpen(aAsset, readFromByAAsset, NULL);
    } else {
        mGif = DGifOpenFileName(fileName, NULL);
    }
    LOGI("mGif:%d", mGif);
    if (mGif) {
        ///初始化化结构图
        DGifSlurp(mGif);
        ///1. 一帧一帧的数据
        ///2. 扩展信息，控制信息
        GraphicsControlBlock gcb;
        long durationMs = 0;
        for (int i = 0; i < mGif->ImageCount; i++) {
            ///获取显示时间
            DGifSavedExtensionToGCB(mGif, i, &gcb);
            durationMs += getDelayMs(gcb);
        }
    }

}


long GifFrame::loadFrame(JNIEnv *env, jobject bitmap, int frameIndex) {

    AndroidBitmapInfo info;
    ///缓冲区像素
    uint32_t *pixels;
    ///使用 AndroidBitmap_getInfo() 从 JNI 检索信息，例如指定位图句柄的宽度和高度
    AndroidBitmap_getInfo(env, bitmap, &info);
    ///用 AndroidBitmap_lockPixels() 锁定像素缓冲区并检索指向它的指针。
    AndroidBitmap_lockPixels(env, bitmap, reinterpret_cast<void **>(&pixels));

    ///在原生代码中，对像素缓冲区进行相应的修改，以使其符合相应像素格式、宽度和其他特性
    SavedImage savedImage = mGif->SavedImages[frameIndex];
    GifImageDesc imageDesc = savedImage.ImageDesc;
    ColorMapObject *colorMapObject = mGif->SColorMap ? mGif->SColorMap : imageDesc.ColorMap;

    for (int i = 0; i < imageDesc.Height; ++i) {
        for (int j = 0; j < imageDesc.Width; ++j) {
            ///将每个像素点的颜色取出来，存到pixels
            int index = i * imageDesc.Width + j;///获取每个像素点的坐标
            GifByteType gifByteType = savedImage.RasterBits[index];
            ///解压
            GifColorType gifColorType = colorMapObject->Colors[gifByteType];///git的每个像素点上的颜色值
            uint32_t color = ARGB_TO_COLOR8888(0xff, gifColorType.Red, gifColorType.Green,gifColorType.Blue);
            pixels[index] = color;
        }
    }

    ///调用 AndroidBitmap_unlockPixels() 来解锁缓冲区

    AndroidBitmap_unlockPixels(env, bitmap);
        GraphicsControlBlock gcb;//获取控制信息
    DGifSavedExtensionToGCB(mGif, frameIndex, &gcb);
    return getDelayMs(gcb);


}

int GifFrame::getFrameCount() {
    return mGif ? mGif->ImageCount : 0;
}

int GifFrame::getWidth() {
    return mGif ? mGif->SWidth : 0;
}

int GifFrame::getHeight() {
    return mGif ? mGif->SHeight : 0;
}



