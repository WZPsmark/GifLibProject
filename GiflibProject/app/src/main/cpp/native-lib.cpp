#include <jni.h>
#include <string>
#include "JavaInputStream.h"
#include "GifFrame.h"



///静态注册native   把gif图片解析 GifFrame 创建一个java的GifFrame 返回给java层
extern "C"
JNIEXPORT jobject JNICALL
Java_com_sk_giflibproject_gif_GifFrame_nativeDecodeStream(JNIEnv *env, jclass clazz, jobject stream,
                                                          jbyteArray buffer) {
    ///首先创建出C++层的GifFrame

    jclass inputStreamClazz = env->FindClass("java/io/InputStream");
    JavaInputStream::readMethodId = env->GetMethodID(inputStreamClazz, "read", "([BII)I");
    JavaInputStream inputStream(env, stream, buffer);
    GifFrame *gifFrame = new GifFrame(&inputStream);
    ///调用Java层的GifFrame的构造函数
    jclass gifFrameClazz = env->FindClass("com/sk/giflibproject/gif/GifFrame");
    jmethodID jmethodId = env->GetMethodID(gifFrameClazz, "<init>", "(JIII)V");
    return env->NewObject(gifFrameClazz, jmethodId,
                          reinterpret_cast<jlong>(gifFrame),
                          gifFrame->getWidth(),
                          gifFrame->getHeight(),
                          gifFrame->getFrameCount());

}


/// 静态注册 native 拿出index帧的数据 loadFrame
extern "C"
JNIEXPORT jlong JNICALL
Java_com_sk_giflibproject_gif_GifFrame_nativeGetFrame(JNIEnv *env, jclass clazz,
                                                      jlong native_handle, jobject bitmap,
                                                      jint frame_index) {

    ///通过句柄拿到C++层GifFrame的对象
    GifFrame *gifFrame = reinterpret_cast<GifFrame *> (native_handle);
    jlong delayMs = gifFrame->loadFrame(env, bitmap, frame_index);
    return delayMs;

}


///动态注册

extern "C"
JNIEXPORT jobject JNICALL
nativeDecodeStreamJNI(JNIEnv *env, jclass clazz, jobject assetManager, jstring gifPath) {
    const char *fileName = env->GetStringUTFChars(gifPath, 0);
    GifFrame *gifFrame = new GifFrame(env, assetManager, fileName);
    env->ReleaseStringUTFChars(gifPath, fileName);
    jclass gifFrameClazz = env->FindClass("com/sk/giflibproject/gif/GifFrame");
    jmethodID jmethodId = env->GetMethodID(gifFrameClazz, "<init>", "(JIII)V");
    return env->NewObject(gifFrameClazz, jmethodId,
                          reinterpret_cast<jlong>(gifFrame),
                          gifFrame->getWidth(),
                          gifFrame->getHeight(),
                          gifFrame->getFrameCount());
}


///声明需要注册的方法列表
JNINativeMethod method[] = {
        {"nativeDecodeStreamJNI", "(Landroid/content/res/AssetManager;Ljava/lang/String;)Lcom/sk/giflibproject/gif/GifFrame;",
                (void *) nativeDecodeStreamJNI
        }
};


jint registNativeMethod(JNIEnv *env) {
    jclass cl = env->FindClass("com/sk/giflibproject/gif/GifFrame");
    if (env->RegisterNatives(cl, method, sizeof(method) / sizeof(method[0])) < 0) {
        return -1;
    }
    return 0;
}

jint unRegistNativeMethod(JNIEnv *env) {
    jclass cl = env->FindClass("com/sk/giflibproject/gif/GifFrame");
    if (env->UnregisterNatives(cl)) {
        return -1;
    }
    return 0;
}

JNIEXPORT jint JNI_OnLoad(JavaVM *vm, void *param) {//动态注册
    JNIEnv *env;
    if (vm->GetEnv(reinterpret_cast<void **>(&env), JNI_VERSION_1_6) == JNI_OK) {
        registNativeMethod(env);
        return JNI_VERSION_1_6;
    } else if (vm->GetEnv(reinterpret_cast<void **>(&env), JNI_VERSION_1_4) == JNI_OK) {
        registNativeMethod(env);
        return JNI_VERSION_1_4;
    }
    return JNI_ERR;

}


JNIEXPORT void JNI_OnUnload(JavaVM *vm, void *param) {//解绑防止溢出

    JNIEnv *env;
    if (vm->GetEnv(reinterpret_cast<void **>(&env), JNI_VERSION_1_6) == JNI_OK) {
        unRegistNativeMethod(env);
    } else if (vm->GetEnv(reinterpret_cast<void **>(&env), JNI_VERSION_1_4) == JNI_OK) {
        unRegistNativeMethod(env);
    }
}
