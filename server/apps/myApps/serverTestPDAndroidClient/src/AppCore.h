#pragma once

#include "ofMain.h"
#include "ofxTCPServer.h"
#include "ofxOsc.h"
#include "AppCore.h"
#include "assert.h"
#include "ofxPd.h"
#include "ofxUI.h"

// a namespace for the Pd types
using namespace pd;

class AppCore : public PdReceiver {

	public:

		// main
		void setup(const int numOutChannels, const int numInChannels,
				   const int sampleRate, const int ticksPerBuffer);
		void update();
		void draw();
        void exit();

		// input callbacks
		void keyPressed(int key);
		void mousePressed(int x, int y, int button);

		// audio callbacks
		void audioReceived(float * input, int bufferSize, int nChannels);
		void audioRequested(float * output, int bufferSize, int nChannels);

		// pd message receiver callbacks
		void print(const std::string& message);

        /*------ Funciones para los clientes Android -------*/

        void sendToast(string toastMess,int clientID);

        /*----ofxPD-----------------------------------------*/
		ofxPd pd;	//< pd instance

        vector<Patch> instances;

        bool bTouch = false;
		float x = 0;
		float y = 0;

        /*----ofxOSC ofxNetwork-----------------------------*/
		ofxTCPServer server;    //Servidor TCP

        //Funciones para los mensajes OSC
        bool getNextMessage(ofxOscMessage * message);
        bool hasWaitingMessages();

        int BUFFER_SIZE = 16384;    //Buffer para los mensajes OSC


        ofTrueTypeFont font;
        ofTrueTypeFont listFont;

        struct client {
            Patch patch;
            string sIp;
            string sName;
            int id;
            bool bTouch = false;
            bool bAudio = false;
            bool bButton = false;
            bool bFader = false;
            bool bAccel = false;
            bool bKeyboard = false;
            ofRectangle rcTouchSw {20,20,50,16};
            ofRectangle rcButtonSw {20,20,50,16};
            ofRectangle rcFaderSw {20,20,50,16};
            ofRectangle rcKeyboardSw {20,20,50,16};
            ofRectangle rcAccelSw {20,20,50,16};
        };

        vector<client> clientList;  //En este vector se almacenan los clientes, para tener una lista de los usuarios conectados

        /*----ofxUI-----------------------------------------*/

        ofxUISuperCanvas * gui;

        void setGUI();

    private:

        //metodos para enviar y recibir paquetes osc, tomados de ofxOsc (para Tesis)
        // queue of osc messages
        std::deque< ofxOscMessage* > messages;


        // socket to listen on
        UdpListeningReceiveSocket* listen_socket;
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

        bool sendTCPMessage( ofxOscMessage& message, int client, ofxTCPServer * server );
        void appendMessage( ofxOscMessage& message, osc::OutboundPacketStream& p );

};
