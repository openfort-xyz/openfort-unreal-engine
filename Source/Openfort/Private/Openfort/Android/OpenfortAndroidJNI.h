#pragma once

#if PLATFORM_ANDROID
#include "Android/AndroidJNI.h"

extern "C"
{
JNI_METHOD void Java_com_epicgames_unreal_GameActivity_handleDeepLink(JNIEnv *, jobject, jstring);

JNI_METHOD void Java_com_epicgames_unreal_GameActivity_handleOnCustomTabsDismissed(JNIEnv *, jobject, jstring);
}

#endif
