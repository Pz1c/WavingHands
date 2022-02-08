#define NATIVE_JAVA_INCLUDED "NATIVE_JAVA_INCLUDED"
#ifdef Q_OS_ANDROID
#include <jni.h>
#include <QDebug>
//#include <QJniObject>


void checkStateFromJava(const QString &url) {
    qDebug() << "checkStateFromJava:" << url;
}


#ifdef __cplusplus
extern "C" {
#endif

JNIEXPORT void JNICALL
  Java_com_kdab_training_MyJavaNatives_checkWarlockProfile(JNIEnv *env,
                                                    jobject obj,
                                                    jstring n)
{
    qDebug() << "Computed fibonacci is:" << n;
    checkStateFromJava("Point 1");
}

#ifdef __cplusplus
}
#endif

// define our native method
static void checkWarlockProfile(JNIEnv */*env*/, jobject /*obj*/, jstring n)
{
    qDebug() << "Computed fibonacci is:" << n;
    checkStateFromJava("Point 2");
//    QJniObject string = QJniObject::callStaticObjectMethod<jstring>(n, "getString");
//    QString qstring = string.toString();


}

// step 2
// create a vector with all our JNINativeMethod(s)
static JNINativeMethod methods[] = {
    { "checkWarlockProfile", // const char* function name;
        "(Ljava/lang/String;)V", // const char* function signature
        (void *)checkWarlockProfile // function pointer
    }
};

// step 1
// this method is called automatically by Java VM
// after the .so file is loaded
JNIEXPORT jint JNI_OnLoad(JavaVM* vm, void* /*reserved*/)
{
    qDebug() << "JNI_OnLoad start";
    JNIEnv* env;
    // get the JNIEnv pointer.
    if (vm->GetEnv(reinterpret_cast<void**>(&env), JNI_VERSION_1_6)
           != JNI_OK) {
        qDebug() << "JNI_OnLoad point1";
        return JNI_ERR;
    }

    // step 3
    // search for Java class which declares the native methods
    jclass javaClass = env->FindClass("com/kdab/training/MyJavaNatives");
    if (!javaClass) {
        qDebug() << "JNI_OnLoad point2";
        return JNI_ERR;
    }
    // step 4
    // register our native methods
    if (env->RegisterNatives(javaClass, methods,
                            sizeof(methods) / sizeof(methods[0])) < 0) {
        qDebug() << "JNI_OnLoad point3";
        return JNI_ERR;
    }
    qDebug() << "JNI_OnLoad point4";
    return JNI_VERSION_1_6;
}
#endif
