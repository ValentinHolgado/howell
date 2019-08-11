#include "testApp.h"

int switchcount = 0;

//--------------------------------------------------------------
void testApp::setup(){
    ofSetWindowTitle("Aplicación Servidor");
    ofBackground(0,64,86);

    transportBGColor = ofColor(44,133,141);

    timeline.setup();
    mainOffset = ofVec2f(5,40); //Variable mainOffset para almacenar el vector de corrimiento de la ventana timeline

    timeline.setOffset(mainOffset); //Corre el timeline en el eje x y en el eje y

    //timeline.setWidth(500);

    //Se cargan las fuentes para el texto de la interfaz grafica
    logoFont.loadFont("GUI/NewMedia Fett.ttf",32);
    transportFont.loadFont("GUI/NewMedia Fett.ttf",18);

    //Se agregan lineas de tiempo
    timeline.addBloques("Bloque");
    timeline.addBloques("Bloque2");
    timeline.addLFO("Lfocito");

    //Setup OSC
    senderosc.setup(HOST,PORT);
    receiverosc.setup(PORT);

	server.setup(44500);
	mutex = CreateMutexA( NULL, FALSE, NULL );
}

//--------------------------------------------------------------
void testApp::update(){
    //Lee los mensajes recibidos por TCP
    for(int i = 0; i < server.getLastID(); i++)
    {
        if(server.isClientConnected(i)) {
            int BUFFER_SIZE = 16384;

            char buffer[BUFFER_SIZE];
            if (server.receiveRawBytes(i,buffer,16384) > 0)
            {
                cout << "Mensaje recibido" << endl;
                osc::ReceivedPacket p(buffer, BUFFER_SIZE);
                osc::ReceivedMessage m(p);
                ProcessMessage(m);

                ofxOscMessage mosc;

                if(hasWaitingMessages())
                {
                    getNextMessage(&mosc);
                    if (mosc.getAddress() == "/touchscreen")
                    {
                        cout << "Mensaje de Cliente ID : " << i << " X= " << mosc.getArgAsInt32(0) << " Y= " << mosc.getArgAsInt32(1) << endl;
                    }
                }
             }
        }
    }
}

//--------------------------------------------------------------
void testApp::draw(){

    /*--- INICIO BARRA DE TRANSPORTE Y LOGO ------------------------*/
    //Dibujar barra de herramientas y logo superior
    ofSetColor(44,133,141,10);
    ofRectangle logo = logoFont.getStringBoundingBox("APLICACION SERVIDOR",10,36);
    ofRect(logo);
    ofSetColor(255,255,203);
    logoFont.drawString("APLICACION SERVIDOR",10,36);
    logo.x += 80; //Separacion entre logo y transporte

    //Barra de transporte, cambia el color si el mouse esta sobre el texto
    if(hoverPlayButton){
        transportTextColor = ofColor(0,255,0);
    }
    else {
        transportTextColor = ofColor(timeline.getColors().textColor);
    }
    playButton = dibujarBotonTransporte("PLAY",logo,playButton,transportTextColor);

    if(hoverPauseButton){
        transportTextColor = ofColor(220,220,0);
    }
    else {
        transportTextColor = ofColor(timeline.getColors().textColor);
    }
    pauseButton = dibujarBotonTransporte("PAUSE",playButton,pauseButton,transportTextColor);

    if(hoverStopButton){
        transportTextColor = ofColor(220,0,0);
    }
    else {
        transportTextColor = ofColor(timeline.getColors().textColor);
    }
    stopButton = dibujarBotonTransporte("STOP",pauseButton,stopButton,transportTextColor);

    if(hoverLoopButton){
        transportTextColor = ofColor(0,220,150);
    }
    else {
        transportTextColor = ofColor(timeline.getColors().textColor);
    }
    loopButton = dibujarBotonTransporte("LOOP",stopButton,loopButton,transportTextColor);
    /*--- FIN BARRA DE TRANSPORTE Y LOGO ------------------------*/

    //Dibujar el timeline
    timeline.draw();
}

//--------------------------------------------------------------
ofRectangle testApp::dibujarBotonTransporte(string name, ofRectangle anterior, ofRectangle actual, ofColor colorTexto) {
    ofSetColor(transportBGColor);
    actual = transportFont.getStringBoundingBox(name,anterior.getX() + anterior.getWidth()+20, mainOffset.y/2 + transportFont.stringHeight(name)/2);
    ofRect(actual);

    ofSetColor(colorTexto);
    transportFont.drawString(name,actual.getX(), mainOffset.y/2 + transportFont.stringHeight(name)/2);

    return actual;
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
    ofxOscMessage m;

    switch (key)
    {
    case 'm':
    case 'M':
        m.setAddress("/touchscreen");
        break;

    case 'b':
    case 'B':
        m.setAddress("/teclado");
        break;

    case 't':
    case 'T':
        m.setAddress("/texto");
        m.addStringArg("Hola cliente");
        break;
    }

    for (int i=0; i < server.getLastID(); i++)
    {
        if(sendTCPMessage(m, i, &server))
        {
            cout << "Mensaje enviado." << endl;
        }
    }
}

//--------------------------------------------------------------
void testApp::keyReleased(int key){

}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){
    //Comprobar que el mouse este sobre los iconos de la barra de transporte
    hoverPlayButton = playButton.inside(x,y);
    hoverPauseButton = pauseButton.inside(x,y);
    hoverStopButton = stopButton.inside(x,y);
    hoverLoopButton = loopButton.inside(x,y);
}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){

}
//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo){

}

void testApp::ProcessMessage( const osc::ReceivedMessage &m)
{
	// convert the message to an ofxOscMessage
	ofxOscMessage* ofMessage = new ofxOscMessage();

	// set the address
	ofMessage->setAddress( m.AddressPattern() );


	// transfer the arguments
	for ( osc::ReceivedMessage::const_iterator arg = m.ArgumentsBegin();
		  arg != m.ArgumentsEnd();
		  ++arg )
	{
		if ( arg->IsInt32() )
			ofMessage->addIntArg( arg->AsInt32Unchecked() );
		else if ( arg->IsInt64() )
			ofMessage->addInt64Arg( arg->AsInt64Unchecked() );
		else if ( arg->IsFloat() )
			ofMessage->addFloatArg( arg->AsFloatUnchecked() );
		else if ( arg->IsString() )
			ofMessage->addStringArg( arg->AsStringUnchecked() );
		else
		{
			cout << "message argument is not int, float, or string" << endl;
		}
	}

	// now add to the queue

	// at this point we are running inside the thread created by startThread,
	// so anyone who calls hasWaitingMessages() or getNextMessage() is coming
	// from a different thread

	// so we have to practise shared memory management

	// grab a lock on the queue
	grabMutex();

	// add incoming message on to the queue
	messages.push_back( ofMessage );

	// release the lock
	releaseMutex();
}

void testApp::grabMutex()
{
#ifdef TARGET_WIN32
	WaitForSingleObject( mutex, INFINITE );
#else
	pthread_mutex_lock( &mutex );
#endif
}

void testApp::releaseMutex()
{
#ifdef TARGET_WIN32
	ReleaseMutex( mutex );
#else
	pthread_mutex_unlock( &mutex );
#endif
}

bool testApp::hasWaitingMessages()
{
	// grab a lock on the queue
	grabMutex();

	// check the length of the queue
	int queue_length = (int)messages.size();

	// release the lock
	releaseMutex();

	// return whether we have any messages
	return queue_length > 0;
}

bool testApp::getNextMessage( ofxOscMessage* message )
{
	// grab a lock on the queue
	grabMutex();

	// check if there are any to be got
	if ( messages.size() == 0 )
	{
		// no: release the mutex
		releaseMutex();
		return false;
	}

	// copy the message from the queue to message
	ofxOscMessage* src_message = messages.front();
	message->copy( *src_message );

	// now delete the src message
	delete src_message;
	// and remove it from the queue
	messages.pop_front();

	// release the lock on the queue
	releaseMutex();

	// return success
	return true;
}

bool testApp::sendTCPMessage( ofxOscMessage& message, int client, ofxTCPServer * server )
{
	static const int OUTPUT_BUFFER_SIZE = 16384;
	char buffer[OUTPUT_BUFFER_SIZE];
    osc::OutboundPacketStream p( buffer, OUTPUT_BUFFER_SIZE );

	// serialise the message
	//p << osc::BeginBundleImmediate;
	appendMessage( message, p );
	//p << osc::EndBundle;

    //server->sendRawBytes( client, p.Data(), p.Size() );
	return server->sendRawBytes( client, p.Data(), p.Size() );
}

void testApp::appendMessage( ofxOscMessage& message, osc::OutboundPacketStream& p )
{
    p << osc::BeginMessage( message.getAddress().c_str() );
	for ( int i=0; i< message.getNumArgs(); ++i )
	{
		if ( message.getArgType(i) == OFXOSC_TYPE_INT32 )
			p << message.getArgAsInt32( i );
		else if ( message.getArgType(i) == OFXOSC_TYPE_INT64 )
			p << (osc::int64)message.getArgAsInt64( i );
		else if ( message.getArgType( i ) == OFXOSC_TYPE_FLOAT )
			p << message.getArgAsFloat( i );
		else if ( message.getArgType( i ) == OFXOSC_TYPE_STRING )
			p << message.getArgAsString( i ).c_str();
		else
		{
			ofLogError() << "bad argument type" + ofToString(message.getArgType( i ));
			assert( false );
		}
	}
	p << osc::EndMessage;
}
