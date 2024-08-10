#include "OpenfortAndroidJNI.h"

#if PLATFORM_ANDROID

#include "Openfort/OpenfortOpenfortSDK.h"
#include "Openfort/OpenfortSubsystem.h"
#include "Engine/GameEngine.h"

UOpenfortOpenfortSDK *GetOpenfortSDK()
{
	UGameEngine *GameEngine = Cast<UGameEngine>(GEngine);

	if (!GameEngine)
	{
		return nullptr;
	}

	UWorld *World = GameEngine ? GameEngine->GetGameWorld() : NULL;

	if (!World)
	{
		return nullptr;
	}

	auto OpenfortSubsystem = World->GetGameInstance()->GetSubsystem<UOpenfortSubsystem>();

	if (!OpenfortSubsystem)
	{
		return nullptr;
	}

	auto OpenfortSDK = OpenfortSubsystem->GetOpenfortSDK();

	if (!OpenfortSDK.IsValid())
	{
		return nullptr;
	}

	return OpenfortSDK.Get();
}

JNI_METHOD void Java_com_epicgames_unreal_GameActivity_handleDeepLink(JNIEnv *env, jobject obj, jstring jDeeplink)
{
	if (env->IsSameObject(jDeeplink, NULL))
	{
		return;
	}

	const char *deeplinkCStr = env->GetStringUTFChars(jDeeplink, NULL);
	const FString deeplink = FString(UTF8_TO_TCHAR(deeplinkCStr));

	if (auto OpenfortSDK = GetOpenfortSDK())
	{
		OpenfortSDK->HandleDeepLink(deeplink);
	}
	env->ReleaseStringUTFChars(jDeeplink, deeplinkCStr);
}

JNI_METHOD void Java_com_epicgames_unreal_GameActivity_handleOnCustomTabsDismissed(JNIEnv *env, jobject obj, jstring jUrl)
{
	if (env->IsSameObject(jUrl, NULL))
	{
		return;
	}

	if (auto OpenfortSDK = GetOpenfortSDK())
	{
		OpenfortSDK->HandleCustomTabsDismissed(FString(UTF8_TO_TCHAR(env->GetStringUTFChars(jUrl, NULL))));
	}
}
#endif