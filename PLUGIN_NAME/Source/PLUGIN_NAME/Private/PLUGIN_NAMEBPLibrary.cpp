#include "PLUGIN_NAMEBPLibrary.h"
#include "PLUGIN_NAME.h"
#include "Async/Async.h"
#include "Engine.h"

#if PLATFORM_ANDROID
#include "Android/AndroidJNI.h"
#include "Android/AndroidApplication.h"
#include "Android/AndroidJava.h"

#include "Android/Utils/AndroidUtils.h"
#endif

#if PLATFORM_IOS

#endif

//==== ������������ ���������� ���������� / Initialization of static variables===========
FTypeDispacth UPLUGIN_NAMEBPLibrary::FStaticValueDispatch;

//==== ���� ������ / Class body==============================================
UPLUGIN_NAMEBPLibrary::UPLUGIN_NAMEBPLibrary(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{

}


//------- ���������� ����� ��������� ���� / Synchronous invocation of native code -------
FString UPLUGIN_NAMEBPLibrary::HelloWorld(FString String)
{
#if PLATFORM_ANDROID	
	/* 
	* "com/epicgames/ue4/TestJavaClass" - package(������������ com/epicgames/ue4) � ��� ������ Java ������ / package (used by com/epicgames/ue4) and the name of your Java class.
	* "HelloWorldOnAndroid" - ��� ����� Java ������� / Name of your Java function.
	* false - ���������� ����� �� ���������� Activity UE4 � Java / Determines whether to pass Activity UE4 to Java.
	* String -
	* ������ ����� ���������� � Java �������(���� ��� ���������� � Java ���� ����������� �������� jobject,
	* ��� ������� ����� ������� ������� ������������������) /
	* A list of your parameters in the Java function(if the variable type in the Java code is specific, such as jobject, it should be converted before calling the function).
	*/
	String = AndroidUtils::CallNativeAndroid<FString>("com/epicgames/ue4/TestJavaClass", "HelloWorldOnAndroid", false, String);

#endif //Android

#if PLATFORM_IOS

#endif// IOS

	return String;
}

//------- ����������� ����� ��������� ���� / Asynchronous calling of native code ------
void UPLUGIN_NAMEBPLibrary::asyncHelloWorld(const FTypeDispacth& CallBackDispatch, FString String)
{
	UPLUGIN_NAMEBPLibrary::FStaticValueDispatch = CallBackDispatch;

#if PLATFORM_ANDROID
	AndroidUtils::CallNativeAndroidVoid("com/epicgames/ue4/TestJavaClass", "asyncHelloWorldOnAndroid", true, String);

#endif //Android

#if PLATFORM_IOS

#endif// IOS
}

//------- �������� ������� Java ���� / Native functions in Java code ---------
#if PLATFORM_ANDROID
JNI_METHOD void Java_com_epicgames_ue4_TestJavaClass_CallBackCPP(JNIEnv* env, jclass clazz, jstring returnStr)
{
	FString result = JavaConvert::FromJavaFString(returnStr);
	UE_LOG(LogTemp, Warning, TEXT("asyncHelloWorld callback caught in C++! - [%s]"), *FString(result));
	UPLUGIN_NAMEBPLibrary::FStaticFunctDispatch(result);//������� ���������
}
#endif //PLATFORM_ANDROID

//-------���� ���������� / Dispatcher body --------------------------
void UPLUGIN_NAMEBPLibrary::FStaticFunctDispatch(FString FValueReturnDispatch)
{
	//������ ������� ��� ����������/Lambda function for the dispatcher
	AsyncTask(ENamedThreads::GameThread, [=]() {
		FStaticValueDispatch.ExecuteIfBound(FValueReturnDispatch);
		});
}

//------- ����� ������������ ��������� / Calling a pop-up message ------------------
void UPLUGIN_NAMEBPLibrary::ShowToastMobile(FString String, ToastLength length)
{
#if PLATFORM_ANDROID
	FString TextForToast = FString("Your message: " + String);

	TextForToast += " Your phone: ";
	TextForToast += FString(AndroidUtils::CallNativeAndroid<FString>("com/epicgames/ue4/TestJavaClass", "getBrand", false));
	TextForToast += " " + FString(AndroidUtils::CallNativeAndroid<FString>("com/epicgames/ue4/TestJavaClass", "getModel", false));
	TextForToast += " ID: ";
	TextForToast += FString(AndroidUtils::CallNativeAndroid<FString>("com/epicgames/ue4/TestJavaClass", "getAndroidId", true));

	AndroidUtils::CallNativeAndroidVoid("com/epicgames/ue4/TestJavaClass", "showToast", true, TextForToast, length);

#endif //Android

#if PLATFORM_IOS

#endif// IOS
}