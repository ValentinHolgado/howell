#include "ofMain.h"
#include "ofApp.h"

int main(){
	ofSetupOpenGL(1024,768, OF_WINDOW);			// <-------- setup the GL context

	// this kicks off the running of my app
	// can be OF_WINDOW or OF_FULLSCREEN
	// pass in width and height too:
	ofRunApp( new ofApp() );
	return 0;
}


#ifdef TARGET_ANDROID
#include <jni.h>

//========================================================================
extern "C"{
	void Java_cc_openframeworks_OFAndroid_init( JNIEnv*  env, jobject  thiz ){
		main();

	}

	void Java_cc_openframeworks_Howell_OFActivity_connectToServer( JNIEnv* env, jobject thiz, jstring ipAddress){

		//Se asigna la IP del servidor que se recibe del dialogo de Android
		//Para los strings provenientes de Java hay que utilizar dos métodos: GetStringUTFChars() y ReleaseStringUTFChars()
		const char *nativeString = env->GetStringUTFChars(ipAddress, 0);
		((ofApp*)ofGetAppPtr())->serverIP = nativeString;
		env->ReleaseStringUTFChars(ipAddress, nativeString);

		//Se llama al método correspondiente después de asignar la IP para conectarse al servidor
		((ofApp*)ofGetAppPtr())->conectar();
	}

	void Java_cc_openframeworks_Howell_OFActivity_sendClientName( JNIEnv* env, jobject thiz, jstring tClientName){
		//Se envía el nombre elegido por el cliente
		const char *nativeString = env->GetStringUTFChars(tClientName, 0);
		((ofApp*)ofGetAppPtr())->clientName = nativeString;
		env->ReleaseStringUTFChars(tClientName, nativeString);

		//Se llama al método correspondiente después de asignar un nombre para el usuario
		((ofApp*)ofGetAppPtr())->conectar();
	}
}
#endif
