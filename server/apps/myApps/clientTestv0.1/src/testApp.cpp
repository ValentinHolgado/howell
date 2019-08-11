#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
    ofSetWindowTitle("CLIENTE");
    botonConectar.set(ofGetWindowWidth()/2-20,ofGetWindowHeight()/2-20,40,40);
    mutex = CreateMutexA( NULL, FALSE, NULL );
}

//--------------------------------------------------------------
void testApp::update(){
    if(connected)
    {
        int BUFFER_SIZE = 16384;

            char buffer[BUFFER_SIZE];
            if (client.receiveRawBytes(buffer,16384) > 0)
            {
                cout << "Mensaje recibido" << endl;
                osc::ReceivedPacket p(buffer, BUFFER_SIZE);
                osc::ReceivedMessage m(p);
                ProcessMessage(m);

                ofxOscMessage mosc;

                if(hasWaitingMessages())
                {
                    getNextMessage(&mosc);
                    if (mosc.getAddress() == "/acelerometro")
                    {
                        swAccelerometer = !swAccelerometer;
                    }
                    else if (mosc.getAddress() == "/teclado")
                    {
                        swKeyboard = !swKeyboard;
                    }
                    else if (mosc.getAddress() == "/touchscreen")
                    {
                        swTouchscreen = !swTouchscreen;
                    }
                    else if (mosc.getAddress() == "/gps")
                    {
                        swGPS = !swGPS;
                    }
                    else if (mosc.getAddress() == "/texto")
                    {
                        str = mosc.getArgAsString(0);
                    }
                }
             }
    }

        conectar();

        if(swAccelerometer)
        {
            //Envia datos del acelerometro
        }

        if(swTouchscreen)
        {
            //Envia datos de la pantalla
            ofxOscMessage m;
            m.setAddress("/touchscreen");
            m.addIntArg(mouseX);
            m.addIntArg(mouseY);

            if(sendTCPMessage(m,&client))
            {
                cout << "Enviando: " << mouseX << " " << mouseY << endl;
            }
        }

        if(swKeyboard)
        {
            //Envia datos del teclado
        }

        if(swGPS)
        {
            //Envia datos de posicion
        }
}

//--------------------------------------------------------------
void testApp::draw(){
    ofDrawBitmapString(str,ofGetWindowWidth()*0.5,ofGetWindowHeight()*0.5);
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){

}

//--------------------------------------------------------------
void testApp::keyReleased(int key){

}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){
    if(botonConectar.inside(x,y))
    {
        hoverConectar = true;
    }
    else
    {
        hoverConectar = false;
    }
}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){
    if(botonConectar.inside(x,y))
    {
        if(hoverConectar)
        {
            connected = client.setup("127.0.0.1", 44500);
            if(connected)
            {
                cout << "Conectado al servidor\n";
            }
        }
    }
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

void testApp::conectar() {
    if(!connected)
    {
        // don't retry all the time, just every few hundred frames or so
        if( retryCounter > 100 )
        {
            connected = client.setup("127.0.0.1", 44500);
            if(connected)
            {
                cout << "Conectado al servidor\n";
            }
            retryCounter = 0;
        } else {
            retryCounter++;
        }
    }
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

bool testApp::sendTCPMessage( ofxOscMessage& message, ofxTCPClient * client )
{
	static const int OUTPUT_BUFFER_SIZE = 16384;
	char buffer[OUTPUT_BUFFER_SIZE];
    osc::OutboundPacketStream p( buffer, OUTPUT_BUFFER_SIZE );

	// serialise the message
	//p << osc::BeginBundleImmediate;
	appendMessage( message, p );
	//p << osc::EndBundle;

    client->sendRawBytes( p.Data(), p.Size() );
	return client->sendRawBytes( p.Data(), p.Size() );
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

