#pragma once

#include "ofMain.h"
#include "ofxNetwork.h"
#include "ofxOsc.h"
#include "assert.h"
#include <windows.h>

class testApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);

        bool getNextMessage(ofxOscMessage * message);
        bool hasWaitingMessages();

		void conectar();

        ofxTCPClient client;

        int retryCounter;
        bool connected;
        ofRectangle botonConectar;
        string str;

        bool hoverConectar;

        bool salirLoop = false;

        //Booleans que funcionan como switches para activar o desactivar el envío de datos
        bool swTouchscreen = false;
        bool swAccelerometer = false;
        bool swKeyboard = false;
        bool swGPS = false;

    private:

        //metodos para enviar y recibir paquetes osc, tomados de ofxOsc (para Tesis)
        // queue of osc messages
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
