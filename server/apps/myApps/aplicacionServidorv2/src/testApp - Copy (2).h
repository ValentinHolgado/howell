#pragma once

#include "ofMain.h"
#include "ofxTimeline.h"
#include "ofxTCPServer.h"
#include "ofxOsc.h"
#include "assert.h"
#include "ofxPd.h"
#include "cliente.h"

#define HOST "localhost"
#define PORT 12345

using namespace pd;

class ServerListenArgs {

};

class testApp : public ofBaseApp, public PdReceiver{

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

		string serverName;
		ofxTextInputField timelineHDuration;
		ofxTextInputField timelineMDuration;
		ofxTextInputField timelineSDuration;

		ofxTimeline timeline;
        void removeBloqueTrack(ofxTLBloqueTrackRemoveEventArgs& args);

		ofxOscSender oscSender;

		ofTrueTypeFont logoFont;
		ofTrueTypeFont transportFont;

		ofColor transportBGColor;

		//OSC por TCP
		ofxTCPServer server;

        bool getNextMessage(ofxOscMessage * message);
        bool hasWaitingMessages();

        int BUFFER_SIZE = 16384;

        /*-----PURE DATA-------*/
        // audio callbacks
		void audioReceived(float * input, int bufferSize, int nChannels);
		void audioRequested(float * output, int bufferSize, int nChannels);

		// pd message receiver callbacks
		void print(const std::string& message);

        /*------ Funciones para los clientes Android -------*/

        void sendToast(string toastMess,int clientID);

        /*----ofxPD-----------------------------------------*/
		ofxPd pd;	//< pd instance
        Patch patch;

        vector<Patch> instances;

        bool bTouch = false;
		float x = 0;
		float y = 0;

        ofTrueTypeFont font;
        ofTrueTypeFont listFont;
        ofTrueTypeFont headerFont;
        ofTrueTypeFont bodyFont;

        struct pista {
            string trackName;
            int clientsAssigned;
        };

        vector<cliente> clientList;  //En este vector se almacenan los clientes, para tener una lista de los usuarios conectados
        vector<pista> pistasList;

        void receivedBloque(ofxTLBloqueEventArgs& bloque);
        void allSensorsOff();

        void enlazarCliente(string _cName, int _cId, string _cIP);
        void desenlazarCliente(int _cId);
        bool tieneCliente();

        ofColor transportTextColor;
        ofRectangle dibujarBotonTransporte(string name, ofRectangle anterior, ofRectangle actual, ofColor colorTexto);

        ofRectangle playButton;
        ofRectangle pauseButton;
        ofRectangle stopButton;
        ofRectangle loopButton;
        ofRectangle addTrackRect;
        ofRectangle serverSwitch;
        ofRectangle cargarButton;
        ofRectangle guardarButton;
        ofRectangle nuevoProyButton;

        bool hoverPlayButton;
        bool hoverPauseButton;
        bool hoverStopButton;
        bool hoverLoopButton;
        bool hoverAddTrackButton;
        bool hoverServerSwitchButton;
        bool hoverCargarButton;
        bool hoverGuardarButton;
        bool hoverNuevoProyButton;

        ofVec2f mainOffset;

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
