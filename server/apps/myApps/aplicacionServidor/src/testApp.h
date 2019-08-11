#pragma once

#include "ofMain.h"
#include "ofxTimeline.h"
#include "ofxTCPServer.h"
#include "ofxOsc.h"

#define HOST "localhost"
#define PORT 12345


class testApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();


		void keyPressed  (int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
		ofxTimeline timeline;

		ofTrueTypeFont logoFont;
		ofTrueTypeFont transportFont;

		ofColor transportBGColor;

		//OSC por TCP
		ofxTCPServer server;

        bool getNextMessage(ofxOscMessage * message);
        bool hasWaitingMessages();

        int BUFFER_SIZE = 16384;

    protected:

        ofColor transportTextColor;
        ofRectangle dibujarBotonTransporte(string name, ofRectangle anterior, ofRectangle actual, ofColor colorTexto);

        ofRectangle playButton;
        ofRectangle pauseButton;
        ofRectangle stopButton;
        ofRectangle loopButton;

        bool hoverPlayButton;
        bool hoverPauseButton;
        bool hoverStopButton;
        bool hoverLoopButton;

        ofVec2f mainOffset;

        //OSC
        ofxOscSender senderosc;
        ofxOscReceiver receiverosc;

		//OSC por TCP
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
