//
// Created by DIY on 2020/5/19.
//

#ifndef GIFLIBPROJECT_GIFFRAME_H
#define GIFLIBPROJECT_GIFFRAME_H

#include <gif_lib.h>
#include "JavaInputStream.h"


class GifFrame {
public:
    GifFrame(JavaInputStream *stream);

    GifFrame(JNIEnv *env, jobject assetManager, const char *fileName);

    ~GifFrame();

    int getWidth();

    int getHeight();

    int getFrameCount();

    long loadFrame(JNIEnv *env, jobject bitmap, int framIndex);

private:
    GifFileType *mGif;

};


#endif //GIFLIBPROJECT_GIFFRAME_H
