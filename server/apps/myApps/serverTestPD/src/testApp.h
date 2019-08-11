#pragma once

#include "ofMain.h"
#include "ofxTCPServer.h"
#include "ofxOsc.h"
#include "AppCore.h"

#include "assert.h"

class testApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();
		void exit();

		void keyPressed  (int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);

        ofxTCPServer server;    //Servidor TCP

        //Funciones para los mnesajes OSC
        bool getNextMessage(ofxOscMessage * message);
        bool hasWaitingMessages();

        int BUFFER_SIZE = 16384;    //Buffer para los mensajes OSC

        AppCore core;   //Nucleo de PureData

        void audioReceived(float * input, int bufferSize, int nChannels);
		void audioRequested(float * output, int bufferSize, int nChannels);

    private:
        ofTrueTypeFont font;
        ofTrueTypeFont listFont;

        struct client {
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

        };

        vector<client> clientList;  //En este vector se almacenan los clientes, para tener una lista de los usuarios conectados

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
