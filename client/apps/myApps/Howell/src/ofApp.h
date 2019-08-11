#pragma once

#include "ofMain.h"
#include "ofxAndroid.h"
#include "ofxNetwork.h"
#include "ofxOsc.h"
#include "assert.h"
#include "ofxXmlSettings.h"
#include "ofxAccelerometer.h"
#include <jni.h>

#define BUFFER_SIZE 16384

class ofApp : public ofxAndroidApp{
	
	public:
		
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		void windowResized(int w, int h);

		void touchDown(int x, int y, int id);
		void touchMoved(int x, int y, int id);
		void touchUp(int x, int y, int id);
		void touchDoubleTap(int x, int y, int id);
		void touchCancelled(int x, int y, int id);
		void swipe(ofxAndroidSwipeDir swipeDir, int id);

		void pause();
		void stop();
		void resume();
		void reloadTextures();

		bool backPressed();
		void okPressed();
		void cancelPressed();

		ofTrueTypeFont font;
		ofTrueTypeFont headerFont;
		ofTrueTypeFont bodyFont;
		ofTrueTypeFont infoFont;
		ofTrueTypeFont butFadFont;

		ofVec3f accel, normAccel;
		string accelmessages[3];

		string infoText;
		string infox;
		string infoy;
		vector<string> infoBF;

		vector<ofRectangle> butRects;

		void updateButtonsRect();

		 //Booleans que funcionan como switches para activar o desactivar el envío de datos
		bool swTouchscreen = false;
		bool swAccelerometer = false;
		bool swKeyboard = false;
		bool swGPS = false;
		bool swButtons = false;
		bool swFaders = false;

		int prevFrame = 0;
		ofColor backgroundColor;
		ofColor keyColor;
		ofColor lightBackgroundColor;
		ofColor lightBlueColor;
		ofxXmlSettings settings;

		int buttons;
		int testButtonCount = 0;	//Eliminar en version final

		int faders;
		float faderValue[4] = {0.0, 0.0, 0.0, 0.0};
		vector<ofRectangle> faderRect;
		vector<ofRectangle> fillFaderRect;

		void updateFadersRect();

		void accelerationChanged(ofPoint &accelval);

		//ofxNetwork
		ofxTCPClient client;
		bool bNameSent = false;
		string clientName;
		int retryCounter;
		void conectar();
		string serverIP = "192.168.0.15";

        //metodos para enviar y recibir paquetes osc, tomados de ofxOsc (para Tesis)
        // queue of osc messages
        bool getNextMessage(ofxOscMessage * message);
        bool hasWaitingMessages();

        std::deque< ofxOscMessage* > messages;

        // socket to listen on
        void ProcessMessage( const osc::ReceivedMessage &m);


#ifdef TARGET_WIN32
        // thread to listen with
        HANDLE thread;
        // mutex for the thread queue
        HANDLE mutex;
#else
        // thread to listen with
        pthread_t thread;
        // mutex for the message queue
        pthread_mutex_t mutex;
#endif
        void grabMutex();
        void releaseMutex();

        bool sendTCPMessage( ofxOscMessage& message, ofxTCPClient * client );
        void appendMessage( ofxOscMessage& message, osc::OutboundPacketStream& p );
};
